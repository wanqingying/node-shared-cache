const addon = require('./addon.js');


// run simple test

console.log(addon.hello()); // 'world'
cache = new addon.ShmCache({ name: "HighScore", size: 1024 * 100 });
cache.set("key1", "value1",3000);
console.log(cache.get("key1"));
setTimeout(() => {
    console.log(cache.get("key1"));
},2998)
setTimeout(() => {
    console.log(cache.get("key1"));
},3002)



