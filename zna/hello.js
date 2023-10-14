var addon = require('./hello.node');

console.log(require('node-addon-api').include)

console.log(addon.hello()); // 'world'

module.exports=addon;

