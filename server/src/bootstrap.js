// clang-format off
const { getOrInitializeCascadedLoader } = internalRequire('internal/modules/esm/loader');
const { defaultLoad, defaultLoadSync } = internalRequire('internal/modules/esm/load');
const { initializeImportMeta } = internalRequire('internal/modules/esm/initialize_import_meta')
const { translators } = internalRequire('internal/modules/esm/translators');
const { ModuleWrap } = internalRequire('internal/test/binding').internalBinding('module_wrap');
const BuildInModule = internalRequire('module');

const path = require('path');
const alt = process._linkedBinding('alt');
const dns = require('dns');
const url = require('url');
const inspector = require('inspector');

(async () => {
  const esmLoader = getOrInitializeCascadedLoader();

  const resource = alt.Resource.current;
  let _exports = null;

  // We need this handler so that NodeJS doesn't
  // crash the process on oncaught exceptions
  process.on('uncaughtException', (err) => {
    alt.logError(`Uncaught exception: ${err.stack ? `${err.stack}` : `${err.message}`}`);
  });

  // Allows users to use 'localhost' address instead of 127.0.0.1 for tcp connections (e.g. database)
  // https://github.com/nodejs/node/issues/40702#issuecomment-958157082
  dns.setDefaultResultOrder('ipv4first');

  try {
    setupImports(esmLoader);

    // Load the global bindings code
    const bindingsGlobal = {};
    new Function('alt', '__global', __internal_bindings_code)(alt, bindingsGlobal);
    __setLogFunction(bindingsGlobal.genericLog);

    const config = alt.Resource.current.config;
    if (config.inspector) {
      const host = config.inspector.host ?? '127.0.0.1';
      const port = config.inspector.port ?? 9229;
      const wait = config.inspector.wait ?? false;

      inspector.open(port, host, wait);
    }

    const extraBootstrapFile = __getExtraBootstrapFile();
    if(extraBootstrapFile.length !== 0) new Function('alt', extraBootstrapFile)(alt);

    // Get the path to the main file for this resource, and load it
    const _path = path.resolve(resource.path, resource.main);
    _exports = await esmLoader.import(url.pathToFileURL(_path).toString(), '', {});
    /* No one used this and only caused problems for people using that function name,
       so let's just remove it for now and see if anyone complains
    if ('start' in _exports) {
      const start = _exports.start;
      if (typeof start === 'function') {
        await start();
      }
    }
    */
  } catch (e) {
    console.error(e);
  }

  __resourceLoaded(resource.name, _exports);
})();

// Sets up our custom way of importing alt:V resources
function setupImports(esmLoader) {
  const altResourceImportPrefix = 'alt';
  const altResourceInternalPrefix = 'altresource';
  const altModuleImportPrefix = 'altinternal';

  const altModules = new Map([
    ['alt', 'alt'],
    ['alt-server','alt'],
    ['alt-shared', 'altShared'],
  ]);

  const modulesCache = new Map();

  translators.set(altResourceInternalPrefix, async function(url) {
    const name = url.slice(altResourceInternalPrefix.length + 1); // Remove prefix
    const exports = alt.Resource.getByName(name).exports;

    // TODO: cache
    return new ModuleWrap(url, undefined, Object.keys(exports), function() {
      for (const exportName in exports) {
        let value;
        try {
          value = exports[exportName];
        } catch {}
        this.setExport(exportName, value);
      }
    });
  });

  translators.set(altModuleImportPrefix, async function(url) {
    const cached = modulesCache.get(url);
    if (cached !== undefined) {
      return cached;
    }

    const name = url.slice(altModuleImportPrefix.length + 1); // Remove prefix
    const bindings = process._linkedBinding(name);

    const exportKeys = Object.keys(bindings);
    const module = new ModuleWrap(url, undefined, [...exportKeys, 'default'], function() {
      for (let i = 0; i < exportKeys.length; ++i) {
        const key = exportKeys[i];
        this.setExport(key, bindings[key]);
      }

      this.setExport('default', bindings);
    });

    modulesCache.set(url, module);
    return module;
  });

  const _warningPackages = {
    'node-fetch': 'Console hangs'
  };

  const altLoader = {
    resolveSync(specifier, context, importAttributes) {
      const altModule = altModules.get(specifier);
      if (altModule !== undefined) {
        return {
          url: `${altModuleImportPrefix}:${altModule}`,
          shortCircuit: true
        };
      }

      const hasAltPrefix = specifier.startsWith(`${altResourceImportPrefix}:`);

      if (alt.hasResource(specifier) && !hasAltPrefix) {
        alt.logWarning(`Trying to import resource '${specifier}' without '${altResourceImportPrefix}:' prefix, this is deprecated behaviour.`);
        alt.logWarning(`Import '${altResourceImportPrefix}:${specifier}' instead to silence this warning.`);

        return {
          url: `${altResourceInternalPrefix}:${specifier}`,
          shortCircuit: true
        };
      }

      if (hasAltPrefix) {
        const specifierWithoutPrefix = specifier.slice(altResourceImportPrefix.length + 1);
        
        if (!alt.hasResource(specifierWithoutPrefix)) {
          alt.logError(`Trying to import resource '${specifierWithoutPrefix}' that doesn't exist`);
        } else {
          return {
            url: `${altResourceInternalPrefix}:${specifierWithoutPrefix}`,
            shortCircuit: true
          };
        }
      }

      if (_warningPackages.hasOwnProperty(specifier)) {
        alt.logWarning(`Using the module '${specifier}' can cause problems. Reason: ${_warningPackages[specifier]}`);
      }

      return esmLoader.defaultResolve(specifier, context, importAttributes);
    },

    resolve(specifier, context, importAttributes) {
      return altLoader.resolveSync(specifier, context, importAttributes);
    },

    loadOrDefault(url, context, loadDefault) {
      if (url.startsWith(`${altResourceInternalPrefix}:`)) {
        return {
          responseURL: url,
          format: altResourceInternalPrefix,
          source: null,
          shortCircuit: true,
        };
      }

      if (url.startsWith(`${altModuleImportPrefix}:`)) {
        return {
          responseURL: url,
          format: altModuleImportPrefix,
          source: null,
          shortCircuit: true,
        };
      }

      return loadDefault(url, context);
    },

    load(url, context) {
      return altLoader.loadOrDefault(url, context, defaultLoad);
    },

    loadSync(url, context) {
      return altLoader.loadOrDefault(url, context, defaultLoadSync);
    },

    importMetaInitialize(meta, context, esmLoader) {
      return initializeImportMeta(meta, context, esmLoader);
    }
  };

  esmLoader.setCustomizations(altLoader);

  // CJS require hook
  const _origModuleLoad = BuildInModule._load;
  BuildInModule._load = function _load(request, parent, isMain) {
    const altModule = altModules.get(request);
    if (altModule !== undefined) {
      return process._linkedBinding(altModule);
    }
  
    return _origModuleLoad(request, parent, isMain);
  }
}

// ***** Utils

// Supresses the warning from NodeJS when importing 'super-internal' modules,
// that the embedder isn't supposed to use
function internalRequire(id) {
  const __emitWarning = process.emitWarning;
  process.emitWarning = () => {};
  const result = require(id);
  process.emitWarning = __emitWarning;
  return result;
}
