// clang-format off
const https = require("https");
const fs = require("fs").promises;
const assert = require("assert");
const pathUtil = require("path");

const ENUM_START = "enum ";
const transpiledEnumNames = new Set();

function transpileTSEnums(tsCode) {
    let jsCode = "";
    let pos = 0;

    while (true) {
        const startIndex = tsCode.indexOf(ENUM_START, pos);
        if (startIndex === -1) {
            // console.log("startIndex -1");
            break;
        }

        const enumNameStartIndex = startIndex + ENUM_START.length;

        let bodyStartIndex = tsCode.indexOf("{", enumNameStartIndex);
        assert.notEqual(bodyStartIndex, -1);

        const bodyEndIndex = tsCode.indexOf("}", bodyStartIndex);
        assert.notEqual(bodyEndIndex, -1);
        pos = bodyEndIndex;

        const body = tsCode
            .slice(bodyStartIndex + 1, bodyEndIndex)
            .replaceAll(/\/\*(.|\n)*\*\//gm, "")
            .replaceAll(/\/\/(.|\n)*$/gm, "");

        const enumName = tsCode
            .slice(enumNameStartIndex, bodyStartIndex)
            .trim();
        if (transpiledEnumNames.has(enumName)) {
            throw new Error(`Detected enum duplicate: ${enumName}`);
        }
        transpiledEnumNames.add(enumName);

        const members = body.split(/(?!"),(?!")/).map((each) => each.trim());

        // console.log({ members } /* "body:\n\n", body, "\n\n" */);

        if (members.at(-1)?.length === 0) {
            // console.log("removing trailing comma space");
            members.pop();
        }

        const jsMembers = [];

        members.reduce((acc, memberContent) => {
            // console.log({
            //   acc,
            //   memberContent,
            // });

            let memberName;
            let memberValue;

            const explicitMember = memberContent.match(/^(.+).*=(.*)$/);
            if (explicitMember) {
                // console.log({ explicitMember });

                [, memberName, memberValue] = explicitMember;
                memberName = memberName.trim();
                memberValue = memberValue.trim();
                if (!(memberValue.includes('"') || memberValue.includes("'"))) {
                    assert.ok(
                        !isNaN(+memberValue),
                        `raw member value: ${memberValue}`
                    );
                    memberValue = +memberValue;
                }
            } else {
                memberName = memberContent;
                assert.ok(
                    typeof acc === "number" && !isNaN(acc),
                    `acc: ${acc} memberName: ${memberName}`
                );
                memberValue = acc + 1;
            }

            // console.log({
            //   memberContent,
            //   memberName,
            //   memberValue,
            // });

            jsMembers.push([memberName.trim(), memberValue]);

            return memberValue;
        }, -1);

        // console.log({
        //   enumName,
        //   members,
        // });

        const jsBody = jsMembers
            .map(([name, value]) => {
                name = name
                    .replaceAll('"', "")
                    .replaceAll("'", "")
                    .replaceAll("\\", "\\\\");
                if (typeof value === "number") {
                    return (
                        `  '${name}': ${value},\n` + `  '${value}': '${name}'`
                    );
                } else {
                    return `  '${name}': ${value},\n` + `  ${value}: '${name}'`;
                }
            })
            .join(",\n");

        jsCode += `export const ${enumName} = {\n${jsBody}\n}\n`;
    }

    return jsCode;
}

const modules = [
    "https://raw.githubusercontent.com/altmp/altv-types/master/shared/index.d.ts",
    "https://raw.githubusercontent.com/altmp/altv-types/master/server/index.d.ts",
    "https://raw.githubusercontent.com/altmp/altv-types/master/client/index.d.ts",
];

(async () => {
    // Base path should point to the main directory of the repo
    if (process.argv.length < 3) {
        showError("Missing 'basePath' argument");
        showUsage();
        process.exit(1);
    }
    const basePath = process.argv[2];

    // for debug purposes
    const outputJsCode = process.argv[3] ?? null;

    let jsCode = "";
    for (const m of modules) {
        const content = await fetchContent(m);
        jsCode += transpileTSEnums(content);
    }

    if (outputJsCode) {
        await fs.writeFile(
            pathUtil.resolve(__dirname, basePath, outputJsCode),
            jsCode
        );
        showLog(
            `Wrote enums transpilation result as JS code to file: ${outputJsCode}`
        );
    }

    // Result bindings output path
    const outputPath = "shared/JSEnums.h";

    // Full output file
    const resultTemplate = `// !!! THIS FILE WAS AUTOMATICALLY GENERATED (ON {Date}), DO NOT EDIT MANUALLY !!!
#pragma once
#include <string>

namespace JSEnums {
    static std::string GetBindingsCode()
    {
        static constexpr char code[] = { {BindingsCode},'\\0' };
        return code;
    }
}
`;

    // Convert the whole file content to a char code array
    const content = jsCode.split("").map((char) => char.charCodeAt(0));
    const outputStr = resultTemplate
        .replace("{BindingsCode}", content.toString())
        .replace("{Date}", `${getDate()} ${getTime()}`);
    await fs.writeFile(
        pathUtil.resolve(__dirname, basePath, outputPath),
        outputStr
    );
    showLog(`Wrote enums transpilation result to file: ${outputPath}`);
})();

function getDate() {
    const date = new Date();
    const day = date.getDate(),
        month = date.getMonth() + 1,
        year = date.getFullYear();
    return `${day < 10 ? `0${day}` : day}/${
        month < 10 ? `0${month}` : month
    }/${year}`;
}

function getTime() {
    const date = new Date();
    const hours = date.getHours(),
        minutes = date.getMinutes(),
        seconds = date.getSeconds();
    return `${hours < 10 ? `0${hours}` : hours}:${
        minutes < 10 ? `0${minutes}` : minutes
    }:${seconds < 10 ? `0${seconds}` : seconds}`;
}

function showLog(...args) {
    console.log(`[${getTime()}]`, ...args);
}

function showError(...args) {
    console.error(`[${getTime()}]`, ...args);
}

function fetchContent(url) {
    return new Promise((resolve, reject) => {
      https.get(url, (response) => {
        let data = '';
  
        // A chunk of data has been received.
        response.on('data', (chunk) => {
          data += chunk;
        });
  
        // The whole response has been received.
        response.on('end', () => {
          resolve(data);
        });
  
      }).on('error', (error) => {
        reject(error);
      });
    });
  }