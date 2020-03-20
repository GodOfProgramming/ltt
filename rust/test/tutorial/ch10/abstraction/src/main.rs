fn largest(list: &[i32]) -> i32 {
  let mut largest = list[0];

  for &item in list {
    if item > largest {
      largest = item;
    }
  }

  largest
}

fn largest_gen<T>(list: &[T]) -> T
where
  T: PartialOrd + Copy,
{
  let mut largest = list[0];

  for &item in list {
    if item > largest {
      largest = item;
    }
  }

  largest
}

fn largest_ref<T>(list: &[T]) -> &T
where
  T: PartialOrd,
{
  let mut largest = &list[0];

  for &item in list {
    if item > *largest {
      *largest = item;
    }
  }

  largest
}

fn main() {
  let v = vec![1, 2, 3];

  let v_largest = largest(&v);

  println!("The largest number in v is {}", v_largest);

  let a = [1, 2, 3];

  let a_largest = largest(&a);

  println!("This largest number in a is {}", a_largest);

  let number_list = vec![1, 2, 3];

  let number_largest = largest_gen(&number_list);

  println!("The largest number in number_list is {}", number_largest);

  let char_list = vec!['a', 'b', 'c'];

  let char_largest = largest_gen(&char_list);

  println!("The largest number in char_largest is {}", char_largest);
}
