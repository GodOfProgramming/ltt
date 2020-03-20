use traits::NewsArticle;
use traits::Summary;
use traits::Tweet;

fn main() {
  let tweet = Tweet {
    username: String::from("horse_ebooks"),
    content: String::from("of course, as you probably already know, people"),
    reply: false,
    retweet: false,
  };

  let article = NewsArticle {
    headline: String::from("Headline"),
    author: String::from("Author"),
    location: String::from("Location"),
    content: String::from("Content"),
  };

  println!("1 new tweet: {}", tweet.summarize());

  println!(
    "1 new article: {}\n{}",
    article.summarize(),
    article.display()
  );
}
