// load.js: add this in a script tag in place of shell.html's {{{ SCRIPT }}}
// to load either WebAssembly or asm.js with a fallback

var script = document.createElement('script');
if (window.WebAssembly) {
    script.src = "craftw.js";
} else {
    (function() {
      var memoryInitializer = 'craft.html.mem';
      if (typeof Module['locateFile'] === 'function') {
        memoryInitializer = Module['locateFile'](memoryInitializer);
      } else if (Module['memoryInitializerPrefixURL']) {
        memoryInitializer = Module['memoryInitializerPrefixURL'] + memoryInitializer;
      }
      var meminitXHR = Module['memoryInitializerRequest'] = new XMLHttpRequest();
      meminitXHR.open('GET', memoryInitializer, true);
      meminitXHR.responseType = 'arraybuffer';
      meminitXHR.send(null);
    })();

    script.src = "craft.js";
}
document.body.appendChild(script);
