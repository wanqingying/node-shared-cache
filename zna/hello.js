
var addon;

if (process.platform === 'win32') {
    addon = require('./hello-win-64.node');
}
else if (process.platform === 'darwin') {
   addon = require('./hello-linux-arm-64');
}else{
    addon = require('./hello-linux-x86-64');
}

console.log(addon.hello()); // 'world'

module.exports=addon;

