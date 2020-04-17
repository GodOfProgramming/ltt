#[macro_export]
macro_rules! count_expr {
    () => { 0 };
    ($_e: expr $(, $rest: expr)*) => { 1 + count_expr!($($rest),*) }
}

#[macro_export]
macro_rules! redis {
  ($cmd:expr $(, $args:expr)* ) => {
    {
      let mut s = format!("*{}\r\n${}\r\n{}\r\n",  1 + count_expr!($($args),*), $cmd.len(), $cmd);
      $(
        s = format!("{}${}\r\n{}\r\n", s, $args.len(), $args);
      )*
      s
    }
  };
}

static SIMPLE_STRING_CODE: u8 = '+' as u8;
static ERROR_CODE: u8 = '-' as u8;
static INTEGER_CODE: u8 = ':' as u8;
static BULK_STRING_CODE: u8 = '$' as u8;
static ARRAY_CODE: u8 = '*' as u8;

#[derive(PartialEq, Debug)]
pub enum Response {
    SimpleString,
    Error,
    Integer,
    BulkString,
    Array,
}

pub fn parse_response(response: &Vec<u8>) -> Result<Response, String> {
    if response.len() > 0 {
        match response[0] {
            x if SIMPLE_STRING_CODE == x => Ok(Response::SimpleString),
            x if ERROR_CODE == x => Ok(Response::Error),
            x if INTEGER_CODE == x => Ok(Response::Integer),
            x if BULK_STRING_CODE == x => Ok(Response::BulkString),
            x if ARRAY_CODE == x => Ok(Response::Array),
            _ => Err(format!("invalid value, code: {}", response[0])),
        }
    } else {
        Err(format!("response length is 0"))
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn redis_get_string() {
        let expected = "*2\r\n$3\r\nGET\r\n$3\r\nfoo\r\n";
        let actual = redis!("GET", "foo");
        assert_eq!(expected, actual);
    }

    #[test]
    fn redis_set_string() {
        let expected = "*3\r\n$3\r\nSET\r\n$3\r\nfoo\r\n$3\r\nbar\r\n";
        let actual = redis!("SET", "foo", "bar");
        assert_eq!(expected, actual);
    }

    #[test]
    fn redis_response_simple_string() {
        let data = "+foobar".as_bytes();
        let data = Vec::from(&data[0..]);
        let actual = parse_response(&data).unwrap();
        assert_eq!(Response::SimpleString, actual);
    }
}
