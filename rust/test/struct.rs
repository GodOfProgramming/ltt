struct Foo {
    value: i8,
}

impl Foo {
    fn foo(&self) -> String {
        format!("value: {}", self.value)
    }
}

fn fnptr<T, F>(obj: T, func: F)
where
    F: Fn(&Foo) -> String,
{
    println!("foo: {}", func(&obj));
}

fn main() {
    let f = Foo { value: 0 };

    println!("foo: {}", Foo::foo(&f));
    fnptr(&f, Foo::foo);
}
