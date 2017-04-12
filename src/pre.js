const args = unescape(document.location.hash).substring(1);
let argv = args === "" ? [] : args.split(' ');
if (argv.length === 0 && window.DEFAULT_ARGV) argv = window.DEFAULT_ARGV;
Module['arguments'] = argv;
