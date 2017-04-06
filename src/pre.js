const args = unescape(document.location.hash).substring(1);
const argv = args === "" ? [] : args.split(' ');
Module['arguments'] = argv;
