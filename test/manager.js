const addon = require('../build/Release/hello.node');
const cluster = require('node:cluster');


class ShmObject {
    cache = null;
    constructor() {
        if (cluster.isMaster || cluster.isPrimary) {
            this.cache = new addon.NodeShareCache("Highscorev4", 1024 * 1024 * 100, true);
        } else {
            this.cache = new addon.NodeShareCache("Highscorev4", 1024 * 1024 * 100, false);
        }
        this.cache.setMaxAge(100);
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
