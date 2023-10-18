const addon = require('../build/Release/hello.node');
const cluster = require('node:cluster');


class ShmObject {
    cache = null;
    constructor() {
        if (cluster.isMaster || cluster.isPrimary) {
            this.cache = new addon.NodeShareCache("Highscorev3", 1024 * 10, true);
        } else {
            this.cache = new addon.NodeShareCache("Highscorev3", 1024 * 10, false);

        }
        this.cache.setMaxAge(50000);
        this.cache.setLock(true);

    }
    get(key) {
        return this.cache.get(key);
    }
    set(key, value) {
        return this.cache.set(key, value);
    }
}

module.exports = ShmObject;
