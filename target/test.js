const addon = require('./addon.js');


// run simple test

console.log(addon.hello()); // 'world'
cache = new addon.ShmCache({ name: "HighScore", size: 1024 * 100, maxAge: 500 });
cache.set("key1", "value1");
console.log(cache.get("key1"));
cache.del("key1");
console.log(cache.get("key1"));



