#!/usr/bin/env ruby
require 'net/smtp'

message = <<MESSAGE_END
From: User1 <jon.p.dickert@gmail.com>
To: User2 <jon.p.dickert@gmail.com>
Subject: SMTP email test

Test message
MESSAGE_END

Net::SMTP.start('localhost') do |smtp|
  smtp.send_message message, 'jon.p.dickert@gmail.com', 'jon.p.dickert@gmail.com'
end

