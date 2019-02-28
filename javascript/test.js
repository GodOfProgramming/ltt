var arr = [
  1,
  2
];

var func = function(n, i) {
  arr[i] = i;
}

console.log(arr);

arr.forEach(func);

console.log(arr);

