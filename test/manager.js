const addon = require('../build/Release/main.node');
// const addon = require('../target/addon');
const cluster = require('node:cluster');



class ShmObject {
    cache = null;
    constructor(config) {
        const { name = "Highscorev4", size = 1024 * 1024 * 20, maxAge = 2000 } = config;
        if (cluster.isMaster || cluster.isPrimary) {
            this.cache = new addon.NodeShareCache(name, size, true);
            process.addListener("exit", () => {
                this.cache.destory();
            })
        } else {
            this.cache = new addon.NodeShareCache(name, size, false);
        }
        this.cache.setMaxAge(maxAge);
        this.cache.setLock(true);
        this.cache.setLogLevel(0);

    }
    get(key) {
        return this.cache.get(key);
    }
    set(key, value) {
        return this.cache.set(key, value);
    }
}

module.exports = ShmObject;
