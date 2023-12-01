const addon = require('./addon.js');


// run simple test

console.log(addon.hello()); // 'world'
cache = new addon.ShmCache({ name: "HighScoreV2", size: 1024 * 1024*20 });
cache.set("key1", "value1", 3000);
cache.set("key2", "value2", 40000);
for (let i = 0; i < 8; i++) {
    // cache.set("key" + i, "value" + i, 4000);
    cache.get("key1")
}

console.log("key1=",cache.get("key1"));
const stat = cache.stat();
console.log('proc mem = ', Math.round(stat.mem_proc / 1024/1024), 'MB');
console.log('used mem = ', Math.round(stat.total_size / 1024/1024), 'MB');

setTimeout(() => {
    console.log("key1=",cache.get("key1"));
    console.log(cache.stat())
}, 2995)
setTimeout(() => {
    console.log("key1=",cache.get("key1"));
    console.log(cache.stat())
}, 3002)



