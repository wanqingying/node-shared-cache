const addon = require('./addon.js');


// run simple test

console.log(addon.hello()); // 'world'
cache = new addon.NodeShareCache("Highscorev4", 1024 * 100, true);
cache.setMaxAge(4000);
cache.setLock(true);
cache.set("key1", "value1");
console.log(cache.get("key1"));


