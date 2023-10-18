const { Event, Suite, Target } = require('benchmark');
const ShmCache = require('./manager.js');
const { exampleJson } = require('./json.js');


const cluster = require('node:cluster');

const Benchmark = require('benchmark');
var suite = new Benchmark.Suite();


const size = Math.ceil(Buffer.byteLength(JSON.stringify(exampleJson)) / 1024);

async function wait(n) {
    return new Promise((resolve, reject) => {
        setTimeout(() => {
            resolve(1);
        }, n);
    });
}

let workerCount = 5;
function waiteExit() {
  let exitCount=0;
  cluster.addListener("exit", (worker, code, signal) => {
    exitCount++;
    if (exitCount >= workerCount) {
      process.exit(0);
    }
  })
}
const kcont = 10;

async function boot() {
    if (cluster.isMaster || cluster.isPrimary) {
        const cache = new ShmCache();

        cache.set('new_key_t', 'value ok');

        cache.set('new_key', JSON.stringify(exampleJson));
        for (let i = 0; i < kcont; i++) {
            cache.set('new_key' + i, JSON.stringify(exampleJson));

        }
        console.log('master init done', cache.get('new_key_t'));

        for (let i = 0; i < workerCount; i++) {
            // await wait(30);
            cluster.fork();
        }
        waiteExit();

    } else {
        const cache = new ShmCache();
        const id = cluster.worker.id;
        await wait(10-id);
        console.info(`work ${id} start `, cache.get('new_key_t'));


        let errCont = 0;
        async function read() {
            for (let i = 0; i < 200; i++) {
                await wait(Math.round(Math.random() * 3));

                // console.log('read-start ' + id);
                const b = cache.get('new_key');
                // console.log('read-end ' + id);


                if (b === 'null') {
                    // console.log('null read', b);
                }
                try {
                    const obj = JSON.parse(b);
                    if (obj.expId !== exampleJson.expId) {
                        errCont++;
                    }
                } catch (e) {
                    errCont++;
                    // console.error(e);
                }
            }
        }
        async function write() {
            for (let i = 0; i < 200; i++) {
                // console.log('set-start ' + id);
                cache.set('new_key', JSON.stringify(exampleJson));
                // console.log('set-end ' + id);

                await wait(Math.round(Math.random() * 3));
            }
        }

        // Promise.all([read(), write()]).finally(() => {
        //   console.log(
        //     `work ${id} done 读数据错误次数=${errCont} 耗时=${
        //       Date.now().valueOf() - _now
        //     }`,
        //   );
        // });

        function runSuit() {
            suite
                .add(
                    'CacheTestRead',
                    async function (deferred) {
                        // await wait();
                        const ki = Math.floor(Math.random() * kcont);

                        try {
                            const b = cache.get('new_key' + ki);
                            try {
                                const obj = JSON.parse(b);
                                if (obj.expId !== exampleJson.expId) {
                                    throw new Error('obj.expId !== exampleJson.expId');
                                }
                            } catch (e) {
                                errCont++;
                            }
                        } catch (e) {
                            console.log(
                                'read new key error',
                                e.message,
                                typeof cache.get('new_key'),
                                cache.get('new_key'),
                            );
                        }finally {
                            // deferred.resolve();
                        }
                    },
                    { defer: false, maxTime: 2 },
                )
                .add(
                    'CacheTestWrite',
                    async function (deferred) {
                        await wait(2);
                        const ki = Math.floor(Math.random() * kcont*100);
                        try {
                            cache.set('new_key' + ki, JSON.stringify(exampleJson));
                        } catch (e) {
                            console.log(
                                'read new key error',
                                e.message,
                                typeof cache.get('new_key'),
                                cache.get('new_key'),
                            );
                        } finally {
                            deferred.resolve();
                        }

                    },
                    { defer: true, maxTime: 2 },
                )
                .on('complete', async function (event) {
                    const res = cache.get('new_key');
                    let obj = {};
                    try {
                        obj = JSON.parse(res);
                    } catch (e) { }

                    console.log(
                        `================== complete process ${id} ${exampleJson.expId}=${obj?.expId}  `,
                    );

                    let read = 0;
                    let write = 0;
                    this.forEach((item) => {
                        if (item.name === 'CacheTestRead') {
                            read += item.hz;
                        } else {
                            write += item.hz;
                        }
                    });
                    console.log(
                        `read : size=${size}KB QPS=${Math.round(read)}  ${Math.round(
                            (read * size) / 1024,
                        )}MB/s`,
                    );
                    console.log(
                        `write: size=${size}KB QPS=${Math.round(write)}  ${Math.round(
                            (write * size) / 1024,
                        )}MB/s`,
                    );
                    console.log(
                        `read+write = ${Math.round(
                            ((write + read) * size) / 1024,
                        )}MB/s errCount=${errCont}`,
                    );
                    process.exit(0);
                })
                .run({ async: false, maxTime: 2 });
        }
        runSuit();
    }
}

boot();