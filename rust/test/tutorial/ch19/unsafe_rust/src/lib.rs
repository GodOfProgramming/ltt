pub fn raw_ptr() {
    let mut num = 0;

    let r1 = &num as *const i32;
    let r2 = &mut num as *const i32;

    unsafe {
        println!("r1 is {}", *r1);
        println!("r2 is {}", *r2);
    }
}

pub fn random_address() {
    let address = 0x12345usize;
    let r = address as *const i32;
}

pub fn unsafe_split() {
    let mut v = vec![1, 2, 3, 4, 5, 6];

    let r = &mut v[..];

    let (a, b) = r.split_at_mut(3);

    assert_eq!(a, &mut [1, 2, 3]);
    assert_eq!(b, &mut [4, 5, 6]);

    let split_at_mut = |slice: &mut [i32], mid: usize| {
        let len = slice.len();
        let ptr = slice.as_mut_ptr();

        assert!(mid <= len);

        use std::slice;

        unsafe {
            (
                slice::from_raw_parts_mut(ptr, mid),
                slice::from_raw_parts_mut(ptr.offset(mid as isize), len - mid),
            )
        }
    };
}

extern "C" {
    fn abs(input: i32) -> i32;
}

pub fn c_code() {
    unsafe {
        println!("Abs of -3 is {} according to C", abs(-3));
    }
}

#[no_mangle]
pub extern "C" fn call_from_c() {
    println!("Just called a rust function from C");
}

unsafe trait Foo {
}

unsafe impl Foo for i32 {
}

