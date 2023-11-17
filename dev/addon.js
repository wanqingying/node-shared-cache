const cluster = require('cluster');
var addon = require('../build/Release/main.node');


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
    set(key, value, maxAge) {
        if (maxAge !== undefined) {
            this.cache.set(key, value, maxAge);
        } else {
            return this.cache.set(key, value);
        }
    }
    del(key) {
        this.cache.del(key);
    }
    setMaxAge(maxAge) {
        this.cache.setMaxAge(maxAge);
    }
    stat() {
        return this.cache.stat();
    }
}

// test()
module.exports = {
    hello: addon.hello,
    NodeShareCache: addon.NodeShareCache,
    ShmCache: ShmCache,
}
