
var addon;

if (process.platform == 'win32') {
    addon = require('./hello-win.node');
}
else if (process.platform == 'darwin') {
   throw new Error('darwin is not supported');
}else{
    addon = require('./hello-linux.node');
}

console.log(addon.hello()); // 'world'

module.exports=addon;

