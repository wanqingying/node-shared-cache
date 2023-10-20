const cluster = require('cluster');
const pkg = require("../package.json");
var addon;

if (process.platform === 'win32') {
    addon = require(`./hello-${pkg.version}-win-64.node`);
} else if (process.platform === 'darwin') {
    addon = require(`./hello-${pkg.version}-linux-arm-64.node`);
} else {
    addon = require(`./hello-${pkg.version}-linux-x86-64.node`);
}

class ShmCache {
    cache = null;
    constructor(config = {}) {
        const { name = "Highscorev4", size = 1024 * 1024 * 20, maxAge = 500, maxSize = 4096 * 1024 * 200 } = config;
        if (cluster.isMaster || cluster.isPrimary) {
            this.cache = new addon.NodeShareCache(name, size, true);
            process.addListener("exit", () => {
                this.cache.destroy();
            })
        } else {
            this.cache = new addon.NodeShareCache(name, size, false);
        }
        this.cache.setMaxAge(maxAge);
        this.cache.setMaxSize(maxSize);
        this.cache.setLock(true);
        this.cache.setLogLevel(2);

    }
    get(key) {
        return this.cache.get(key);
    }
    set(key, value) {
        return this.cache.set(key, value);
    }
    setMaxAge(maxAge) {
        this.cache.setMaxAge(maxAge);
    }
    destroy(){
        this.cache.destroy();
    }
}

// test()
module.exports = {
    hello: addon.hello,
    NodeShareCache: addon.NodeShareCache,
    ShmCache: ShmCache,
}
