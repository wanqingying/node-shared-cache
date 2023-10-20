const fs=require('fs')
const pkg=require('../package.json')

let p="main.node"

if (process.platform === 'win32') {
    p=`hello-${pkg.version}-win-64.node`
} else if (process.platform === 'darwin') {
    p=`hello-${pkg.version}-linux-arm-64.node`
} else {
    p=`hello-${pkg.version}-linux-x86-64.node`
}


// copy file ../build/Release/hello.node to ../target/hello.node
fs.copyFileSync('../build/Release/main.node',`../target/${p}`)



