const Reps = 10000;
const NumItems = 16000;

function run(before, f, after) {
	before();
	let then = new Date;
	for (let i = 0; i < Reps; i++) {
		f();
	}
	let now = new Date;
	console.log("time = ", now - then);
	after();
}

var arr = []
for (let i = 0; i < NumItems; i++) {
	arr.push([{ prop: i }, { other: NumItems - i }, i & 1 === 0]);
}

var arr1 = [];
var arr2 = [];

run(function() {
	arr1 = [];
	arr2 = [];
}, function() {
	arr1 = arr.filter(function(pt) {
		return pt[2] === true;
	});

	arr2 = arr.filter(function(pt) {
		return pt[2] === false;
	});
}, function() {
});

console.log("arr1 length = ", arr1.length);
console.log("arr2 length = ", arr2.length);

arr1 = [];
arr2 = [];

run(function() {
	for (let i = 0; i < arr.length; i++) {
		(arr[i][2] === true ? arr1 : arr2).push(arr[i]);
	}
});

console.log("arr1 length = ", arr1.length);
console.log("arr2 length = ", arr2.length);

