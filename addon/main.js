
var addon;

if (process.platform === 'win32') {
    addon = require('./hello-win-64.node');
}
else if (process.platform === 'darwin') {
    addon = require('./hello-linux-arm-64');
} else {
    addon = require('./hello-linux-x86-64');
}

console.log(addon.hello()); // 'world'
const cache = new addon.NodeShareCache("Highscorev4", 1024 * 100, true);




module.exports = function () {
    cache.setMaxAge(4000);
    cache.setLock(true);
    cache.set("key1", "value1");
    return "hello test NodeShareCache key1=" + cache.get("key1");
}

