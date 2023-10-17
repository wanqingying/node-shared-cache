var addon = require('bindings')('hello');

async function wait(n) {
    return new Promise((resolve, reject) => {
        setTimeout(() => {
            resolve(1);
        }, n);
    });
}

// addon.open(true)
// console.log(require('node-addon-api').include)

async function test() {
  console.log('any = ',addon.getAnyType())
}

test().finally(() => {

    // console.log("keyv1=", addon.get("keyv1"))

    console.log('end')
})




module.exports = addon;


var json1 = {
    expId: 3433,
    mainPackage: {
      appId: 'ks707065143182458884',
      versionName: '1.10.10',
      versionCode: 148,
      compilerVersion: '1.10.5-alpha.10',
      url: 'https://blobstore-nginx.staging.kuaishou.com/bs2/mp-packages/development-ks707065143182458884-dist-9481b8c039661dc972fc3bffc886409b.zip',
      cdnURLs: [
        {
          cdn: 'blobstore-nginx.staging.kuaishou.com',
          path: '/bs2/mp-packages/development-ks707065143182458884-dist-9481b8c039661dc972fc3bffc886409b.zip',
        },
        {
          cdn: 'js2.a.kwimgs.com',
          path: '/bs2/mp-packages/development-ks707065143182458884-dist-9481b8c039661dc972fc3bffc886409b.zip',
        },
        {
          cdn: 'blobstore-nginx.staging.kuaishou.com',
          path: '/bs2/mp-packages/development-ks707065143182458884-dist-9481b8c039661dc972fc3bffc886409b.zip',
        },
      ],
      updateTime: '1692882634289',
      md5: '9481b8c039661dc972fc3bffc886409b',
      size: 1827256,
      releaseCode: 2317,
    },
    subPackages: [
      {
        updateTime: '1692882634505',
        url: 'https://blobstore-nginx.staging.kuaishou.com/bs2/mp-packages/development-ks707065143182458884-dist-b2f17de5d337508479548f519922ec00.zip',
        cdnURLs: [
          {
            cdn: 'blobstore-nginx.staging.kuaishou.com',
            path: '/bs2/mp-packages/development-ks707065143182458884-dist-b2f17de5d337508479548f519922ec00.zip',
          },
          {
            cdn: 'js2.a.kwimgs.com',
            path: '/bs2/mp-packages/development-ks707065143182458884-dist-b2f17de5d337508479548f519922ec00.zip',
          },
          {
            cdn: 'blobstore-nginx.staging.kuaishou.com',
            path: '/bs2/mp-packages/development-ks707065143182458884-dist-b2f17de5d337508479548f519922ec00.zip',
          },
        ],
        root: '/page/finisAutoApi/packageA/',
        name: '',
        md5: 'b2f17de5d337508479548f519922ec00',
        size: 6292,
      },
      {
        updateTime: '1692882634713',
        url: 'https://blobstore-nginx.staging.kuaishou.com/bs2/mp-packages/development-ks707065143182458884-dist-0e413b6d52cb5a3474eaf16b6177ea78.zip',
        cdnURLs: [
          {
            cdn: 'blobstore-nginx.staging.kuaishou.com',
            path: '/bs2/mp-packages/development-ks707065143182458884-dist-0e413b6d52cb5a3474eaf16b6177ea78.zip',
          },
          {
            cdn: 'js2.a.kwimgs.com',
            path: '/bs2/mp-packages/development-ks707065143182458884-dist-0e413b6d52cb5a3474eaf16b6177ea78.zip',
          },
          {
            cdn: 'blobstore-nginx.staging.kuaishou.com',
            path: '/bs2/mp-packages/development-ks707065143182458884-dist-0e413b6d52cb5a3474eaf16b6177ea78.zip',
          },
        ],
        root: '/page/finisAutoApi/packageB/',
        name: '',
        md5: '0e413b6d52cb5a3474eaf16b6177ea78',
        size: 1737,
      },
      {
        updateTime: '1692882634489',
        url: 'https://blobstore-nginx.staging.kuaishou.com/bs2/mp-packages/development-ks707065143182458884-dist-d364348db90f7579384992eb5517770e.zip',
        cdnURLs: [
          {
            cdn: 'blobstore-nginx.staging.kuaishou.com',
            path: '/bs2/mp-packages/development-ks707065143182458884-dist-d364348db90f7579384992eb5517770e.zip',
          },
          {
            cdn: 'js2.a.kwimgs.com',
            path: '/bs2/mp-packages/development-ks707065143182458884-dist-d364348db90f7579384992eb5517770e.zip',
          },
          {
            cdn: 'blobstore-nginx.staging.kuaishou.com',
            path: '/bs2/mp-packages/development-ks707065143182458884-dist-d364348db90f7579384992eb5517770e.zip',
          },
        ],
        root: '/page/finishAutoCase/packageA/',
        name: '',
        md5: 'd364348db90f7579384992eb5517770e',
        size: 2808,
      },
      {
        updateTime: '1692882634518',
        url: 'https://blobstore-nginx.staging.kuaishou.com/bs2/mp-packages/development-ks707065143182458884-dist-206a8ab945db50f650c05a82ee6551ad.zip',
        cdnURLs: [
          {
            cdn: 'blobstore-nginx.staging.kuaishou.com',
            path: '/bs2/mp-packages/development-ks707065143182458884-dist-206a8ab945db50f650c05a82ee6551ad.zip',
          },
          {
            cdn: 'js2.a.kwimgs.com',
            path: '/bs2/mp-packages/development-ks707065143182458884-dist-206a8ab945db50f650c05a82ee6551ad.zip',
          },
          {
            cdn: 'blobstore-nginx.staging.kuaishou.com',
            path: '/bs2/mp-packages/development-ks707065143182458884-dist-206a8ab945db50f650c05a82ee6551ad.zip',
          },
        ],
        root: '/page/finishAutoCase/packageB/',
        name: '',
        md5: '206a8ab945db50f650c05a82ee6551ad',
        size: 4150,
      },
    ],
    appInfo: {
      appId: 'ks707065143182458884',
      name: 'testcase1',
      desc: 'testcase1',
      icon: 'http://media-cloud-api.staging.kuaishou.com/kimg/kimg/bs2/zt-image-host/CitDaFl3T0dabVpHUmhOREF4TVRCa1pHUmlZbVZrWWpBMUVKYk0xeTgucG5nEJbM1y8:204x204.png',
      developerName: '自动化测试公司',
      netDomains: {
        request: [
          'qa-mp.corp.kuaishou.com',
          'www.baidu.com',
          'cdnfile.corp.kuaishou.com',
          'sso.corp.kuaishou.com',
          'ks-mp-server.staging.kuaishou.com',
          'ks-mp-server.test.gifshow.com',
          'wikipedia.org',
          'paddlepaddle.org.cn',
          'open.kuaishou.com',
        ],
        uploadFile: ['cdn.corp.kuaishou.com'],
        downloadFile: ['static.yximgs.com'],
        socket: ['echo.websocket.org'],
        udp: [],
      },
      scopeName: [
        'scope.ul.camera',
        'scope.ul.location',
        'scope.ul.album',
        'scope.ul.locationBackground',
        'scope.ul.record',
        'scope.ks.poi',
        'scope.ks.pay',
        'scope.ks.product',
        'scope.ks.play',
        'ad_leads',
        'scope.tp.settings',
        'scope.ks.course',
        'scope.us.profile',
        'scope.ks.daren_data',
        'scope.us.phone',
        'scope.ks.video',
        'scope.ks.epay',
        'scope.ks.daren_task',
        'scope.ks.ai_painting',
        'scope.ks.feed',
        'scope.ks.live.interaction.play',
        'scope.ks.leads',
        'scope.ks.spark_task',
        'scope.ul.deviceinfo',
        'scope.ks.spark_settle_data',
        'ad_insurance',
      ],
      menuBlocklist: ['watchLater'],
      isInternal: false,
      webViewDomains: [
        'cdnfile.corp.kuaishou.com',
        'qa-mp.corp.kuaishou.com',
        'sso.corp.kuaishou.com',
        '*.baidu.com',
        '*.paddlepaddle.org.cn',
        '*.qq.com',
        'wikipedia.org',
        'paddlepaddle.org.cn',
      ],
      subType: 1,
      customMenu: [],
      screenEntry: 0,
      isIntegrated: true,
      canShareExternal: true,
      shareExternalSubbiz: 'XCX_AI',
      appEnableGuide: true,
      apiBlacklist: '',
    },
    framework: {
      versionCode: '4325',
      versionName: '1.60.6-test.ff1437',
      url: 'https://js2.a.kwimgs.com/udata/pkg/KMA/kma/development/dbd1fd0c63307a8125b34d04e0ff1437/1.60.6-test.ff1437.zip',
      cdnURLs: [
        {
          cdn: 'blobstore-nginx.staging.kuaishou.com',
          path: '/udata/pkg/KMA/kma/development/dbd1fd0c63307a8125b34d04e0ff1437/1.60.6-test.ff1437.zip',
        },
        {
          cdn: 'js2.a.kwimgs.com',
          path: '/udata/pkg/KMA/kma/development/dbd1fd0c63307a8125b34d04e0ff1437/1.60.6-test.ff1437.zip',
        },
        {
          cdn: 'blobstore-nginx.staging.kuaishou.com',
          path: '/udata/pkg/KMA/kma/development/dbd1fd0c63307a8125b34d04e0ff1437/1.60.6-test.ff1437.zip',
        },
      ],
      md5: 'dbd1fd0c63307a8125b34d04e0ff1437',
      desc: 'branch: test/yyyyy @ 697ec80291c79493d4091977b3dd787b15d7bb7d\nadapter size:\n- blank      : 1865 B\n- ios        : 31229 B\n- j2v8       : 24208 B\n- ksWebview  : 192882 B\n- simulator  : 9141 B\n- web        : 4106 B',
      size: '4025472',
      downloadPriority: 'low',
    },
    mainPackage2: {
      appId: 'ks707065143182458884',
      versionName: '1.10.10',
      versionCode: 148,
      compilerVersion: '1.10.5-alpha.10',
      url: 'https://blobstore-nginx.staging.kuaishou.com/bs2/mp-packages/development-ks707065143182458884-dist-9481b8c039661dc972fc3bffc886409b.zip',
      cdnURLs: [
        {
          cdn: 'blobstore-nginx.staging.kuaishou.com',
          path: '/bs2/mp-packages/development-ks707065143182458884-dist-9481b8c039661dc972fc3bffc886409b.zip',
        },
        {
          cdn: 'js2.a.kwimgs.com',
          path: '/bs2/mp-packages/development-ks707065143182458884-dist-9481b8c039661dc972fc3bffc886409b.zip',
        },
        {
          cdn: 'blobstore-nginx.staging.kuaishou.com',
          path: '/bs2/mp-packages/development-ks707065143182458884-dist-9481b8c039661dc972fc3bffc886409b.zip',
        },
      ],
      updateTime: '1692882634289',
      md5: '9481b8c039661dc972fc3bffc886409b',
      size: 1827256,
      releaseCode: 2317,
    },
    subPackages2: [
      {
        updateTime: '1692882634505',
        url: 'https://blobstore-nginx.staging.kuaishou.com/bs2/mp-packages/development-ks707065143182458884-dist-b2f17de5d337508479548f519922ec00.zip',
        cdnURLs: [
          {
            cdn: 'blobstore-nginx.staging.kuaishou.com',
            path: '/bs2/mp-packages/development-ks707065143182458884-dist-b2f17de5d337508479548f519922ec00.zip',
          },
          {
            cdn: 'js2.a.kwimgs.com',
            path: '/bs2/mp-packages/development-ks707065143182458884-dist-b2f17de5d337508479548f519922ec00.zip',
          },
          {
            cdn: 'blobstore-nginx.staging.kuaishou.com',
            path: '/bs2/mp-packages/development-ks707065143182458884-dist-b2f17de5d337508479548f519922ec00.zip',
          },
        ],
        root: '/page/finisAutoApi/packageA/',
        name: '',
        md5: 'b2f17de5d337508479548f519922ec00',
        size: 6292,
      },
      {
        updateTime: '1692882634713',
        url: 'https://blobstore-nginx.staging.kuaishou.com/bs2/mp-packages/development-ks707065143182458884-dist-0e413b6d52cb5a3474eaf16b6177ea78.zip',
        cdnURLs: [
          {
            cdn: 'blobstore-nginx.staging.kuaishou.com',
            path: '/bs2/mp-packages/development-ks707065143182458884-dist-0e413b6d52cb5a3474eaf16b6177ea78.zip',
          },
          {
            cdn: 'js2.a.kwimgs.com',
            path: '/bs2/mp-packages/development-ks707065143182458884-dist-0e413b6d52cb5a3474eaf16b6177ea78.zip',
          },
          {
            cdn: 'blobstore-nginx.staging.kuaishou.com',
            path: '/bs2/mp-packages/development-ks707065143182458884-dist-0e413b6d52cb5a3474eaf16b6177ea78.zip',
          },
        ],
        root: '/page/finisAutoApi/packageB/',
        name: '',
        md5: '0e413b6d52cb5a3474eaf16b6177ea78',
        size: 1737,
      },
      {
        updateTime: '1692882634489',
        url: 'https://blobstore-nginx.staging.kuaishou.com/bs2/mp-packages/development-ks707065143182458884-dist-d364348db90f7579384992eb5517770e.zip',
        cdnURLs: [
          {
            cdn: 'blobstore-nginx.staging.kuaishou.com',
            path: '/bs2/mp-packages/development-ks707065143182458884-dist-d364348db90f7579384992eb5517770e.zip',
          },
          {
            cdn: 'js2.a.kwimgs.com',
            path: '/bs2/mp-packages/development-ks707065143182458884-dist-d364348db90f7579384992eb5517770e.zip',
          },
          {
            cdn: 'blobstore-nginx.staging.kuaishou.com',
            path: '/bs2/mp-packages/development-ks707065143182458884-dist-d364348db90f7579384992eb5517770e.zip',
          },
        ],
        root: '/page/finishAutoCase/packageA/',
        name: '',
        md5: 'd364348db90f7579384992eb5517770e',
        size: 2808,
      },
      {
        updateTime: '1692882634518',
        url: 'https://blobstore-nginx.staging.kuaishou.com/bs2/mp-packages/development-ks707065143182458884-dist-206a8ab945db50f650c05a82ee6551ad.zip',
        cdnURLs: [
          {
            cdn: 'blobstore-nginx.staging.kuaishou.com',
            path: '/bs2/mp-packages/development-ks707065143182458884-dist-206a8ab945db50f650c05a82ee6551ad.zip',
          },
          {
            cdn: 'js2.a.kwimgs.com',
            path: '/bs2/mp-packages/development-ks707065143182458884-dist-206a8ab945db50f650c05a82ee6551ad.zip',
          },
          {
            cdn: 'blobstore-nginx.staging.kuaishou.com',
            path: '/bs2/mp-packages/development-ks707065143182458884-dist-206a8ab945db50f650c05a82ee6551ad.zip',
          },
        ],
        root: '/page/finishAutoCase/packageB/',
        name: '',
        md5: '206a8ab945db50f650c05a82ee6551ad',
        size: 4150,
      },
    ],
    appInfo2: {
      appId: 'ks707065143182458884',
      name: 'testcase1',
      desc: 'testcase1',
      icon: 'http://media-cloud-api.staging.kuaishou.com/kimg/kimg/bs2/zt-image-host/CitDaFl3T0dabVpHUmhOREF4TVRCa1pHUmlZbVZrWWpBMUVKYk0xeTgucG5nEJbM1y8:204x204.png',
      developerName: '自动化测试公司',
      netDomains: {
        request: [
          'qa-mp.corp.kuaishou.com',
          'www.baidu.com',
          'cdnfile.corp.kuaishou.com',
          'sso.corp.kuaishou.com',
          'ks-mp-server.staging.kuaishou.com',
          'ks-mp-server.test.gifshow.com',
          'wikipedia.org',
          'paddlepaddle.org.cn',
          'open.kuaishou.com',
        ],
        uploadFile: ['cdn.corp.kuaishou.com'],
        downloadFile: ['static.yximgs.com'],
        socket: ['echo.websocket.org'],
        udp: [],
      },
      scopeName: [
        'scope.ul.camera',
        'scope.ul.location',
        'scope.ul.album',
        'scope.ul.locationBackground',
        'scope.ul.record',
        'scope.ks.poi',
        'scope.ks.pay',
        'scope.ks.product',
        'scope.ks.play',
        'ad_leads',
        'scope.tp.settings',
        'scope.ks.course',
        'scope.us.profile',
        'scope.ks.daren_data',
        'scope.us.phone',
        'scope.ks.video',
        'scope.ks.epay',
        'scope.ks.daren_task',
        'scope.ks.ai_painting',
        'scope.ks.feed',
        'scope.ks.live.interaction.play',
        'scope.ks.leads',
        'scope.ks.spark_task',
        'scope.ul.deviceinfo',
        'scope.ks.spark_settle_data',
        'ad_insurance',
      ],
      menuBlocklist: ['watchLater'],
      isInternal: false,
      webViewDomains: [
        'cdnfile.corp.kuaishou.com',
        'qa-mp.corp.kuaishou.com',
        'sso.corp.kuaishou.com',
        '*.baidu.com',
        '*.paddlepaddle.org.cn',
        '*.qq.com',
        'wikipedia.org',
        'paddlepaddle.org.cn',
      ],
      subType: 1,
      customMenu: [],
      screenEntry: 0,
      isIntegrated: true,
      canShareExternal: true,
      shareExternalSubbiz: 'XCX_AI',
      appEnableGuide: true,
      apiBlacklist: '',
    },
    framework2: {
      versionCode: '4325',
      versionName: '1.60.6-test.ff1437',
      url: 'https://js2.a.kwimgs.com/udata/pkg/KMA/kma/development/dbd1fd0c63307a8125b34d04e0ff1437/1.60.6-test.ff1437.zip',
      cdnURLs: [
        {
          cdn: 'blobstore-nginx.staging.kuaishou.com',
          path: '/udata/pkg/KMA/kma/development/dbd1fd0c63307a8125b34d04e0ff1437/1.60.6-test.ff1437.zip',
        },
        {
          cdn: 'js2.a.kwimgs.com',
          path: '/udata/pkg/KMA/kma/development/dbd1fd0c63307a8125b34d04e0ff1437/1.60.6-test.ff1437.zip',
        },
        {
          cdn: 'blobstore-nginx.staging.kuaishou.com',
          path: '/udata/pkg/KMA/kma/development/dbd1fd0c63307a8125b34d04e0ff1437/1.60.6-test.ff1437.zip',
        },
      ],
      md5: 'dbd1fd0c63307a8125b34d04e0ff1437',
      desc: 'branch: test/yyyyy @ 697ec80291c79493d4091977b3dd787b15d7bb7d\nadapter size:\n- blank      : 1865 B\n- ios        : 31229 B\n- j2v8       : 24208 B\n- ksWebview  : 192882 B\n- simulator  : 9141 B\n- web        : 4106 B',
      size: '4025472',
      downloadPriority: 'low',
    },
  };
  
  const json2 = {
    name: 'dbd1fd0c63307a8125b34d04e0ff1437',
    name2: 'dbd1fd0c63307a8125b34d04e0ff1437',
    name3: 'dbd1fd0c63307a8125b34d04e0ff1437',
  };
  
  

