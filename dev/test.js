const addon = require('./addon.js');


// run simple test

console.log(addon.hello()); // 'world'
cache = new addon.ShmCache({ name: "HighScore", size: 1024 * 100 });
cache.set("key1", "value1",3000);
cache.set("key2", "value2",4000);
for (let i = 0; i < 100000; i++) {
    cache.set("key"+i, "value"+i,4000);
}

console.log(cache.get("key1"));
console.log(cache.stat());
setTimeout(() => {
    console.log(cache.get("key1"));
},2998)
setTimeout(() => {
    console.log(cache.get("key1"));
},3002)



