const cluster = require('cluster');
var addon;

if (process.platform === 'win32') {
    addon = require('./hello-win-64-v3.node');
} else if (process.platform === 'darwin') {
    addon = require('./hello-linux-arm-64.node');
} else {
    addon = require('./hello-linux-x86-64-v3.node');
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
}

// test()
module.exports = {
    hello: addon.hello,
    NodeShareCache: addon.NodeShareCache,
    ShmCache: ShmCache,
}
