function fib(count) {
	var prev = 0;
	var curr = 1;

	for (var i = 0; i < count; i++) {
		var tmp = curr;
		curr += prev;
		prev = tmp;
	}

	return prev;
}

argv = process.argv

if (argv.length < 3) {
	console.log("You need to supply an argument");
	process.exit(1);
}

var count = +argv[3]

if (count < 0) {
	console.log("You need to use a positve number");
	process.exit(1);
}

var result = fib(count);

console.log(result);
