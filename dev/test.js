const addon = require('./addon.js');


// run simple test

console.log(addon.hello()); // 'world'
cache = new addon.ShmCache({ name: "HighScore", size: 1024 * 1024*20 });
cache.set("key1", "value1", 3000);
cache.set("key2", "value2", 4000);
for (let i = 0; i < 800000; i++) {
    // cache.set("key" + i, "value" + i, 4000);
    cache.get("key1")
}

console.log(cache.get("key1"));
const stat = cache.stat();
console.log('proc mem = ', Math.round(stat.mem_proc / 1024), 'kb');
console.log('used mem = ', Math.round(stat.total_size / 1024), 'kb');

// setTimeout(() => {
//     console.log(cache.get("key1"));
// }, 2998)
// setTimeout(() => {
//     console.log(cache.get("key1"));
// }, 3002)



