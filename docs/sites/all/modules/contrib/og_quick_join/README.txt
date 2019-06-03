
Organic Groups Quick Join 

This Module allows for people to quickly join as users to your Drupal site 
through a form on a block.  The typical use case is to be able to 
join one or more Organic Groups so as to receive messages from
the organic group via the OG Mailing List module.  This module is meant to
make Drupal work like MailChimp or another e-mail listserv mailing list.


Initial Configuration:

Typically, you'll want to place the block and enable it, and then
select the groups and roles you want to be available for the user to join or 
to automatically be added to.  You may also want to add a reCaptacha
to form submissions for OG Quick Join, and also enable User Verify
so that people can't randomly add others to the group/mailing list.

Helpful Modules to use with this Module:

OG Mailing List (http://drupal.org/project/og_mailinglist)
reCAPTCHA   (http://drupal.org/project/recaptcha)
User Verification (http://drupal.org/project/user_verify)  Note - User Verify conflicts with LoginToboggan for now

Here is example subject and Text to use if you're using the User Verification module.  This module makes the
module work a lot more like MailChimp since OG Mailing list won't send mail to blocked users, and you can
configure User Verification to have new members be blocked.  The only challenge with this is that
there's not a good way to tell the user to log in and update their password.  They likely will need to 
issue a password reset request.

Subject:   [site:name] - please confirm your subscriptions

Body:

Hi, [user:name],

Thank you for joining the mailing lists at [site:name]. To confirm that this was actually you, please click this link or copy and paste it to your browser:

[user-verification:link]

You can configure your mailing list settings at http://bitcoindc.com/user/[user:uid]/mailinglist

--  [site:name] team


If you're not using User Verification, paste this into the text for your Welcome message.  Messages will still 
be sent out to users in this case, but you'll be able to see if they ever logged in and delete them if they haven't.

Subject:   [site:name] - please confirm your subscriptions

Body:

[user:name],

A site administrator at [site:name] has created an account for you. You may now log in by clicking this link or copying and pasting it to your browser:

[user:one-time-login-url]

This link can only be used once to log in and will lead you to a page where you can set your password.

After setting your password, you will be able to log in at [site:login-url] in the future using:

username: [user:name]
password: Your password

--  [site:name] team
