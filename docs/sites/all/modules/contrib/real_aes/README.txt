# Real AES

## Introduction

Real AES provides an encryption method plugin for the Encrypt module (https://drupal.org/project/encrypt).
It also serves as a library loader for the Defuse PHP-encryption library.

Defuse PHP-encryption provides authenticated encryption via an Encrypt-then-MAC scheme. AES-256 CBC is the symmetric
encryption algorithm, SHA-256 the hash algorithm for the HMAC. IV's are automatically and randomly generated. You do
not need to manage the IV separately, as it is included in the ciphertext.

Ciphertext format is: HMAC || iv || ciphertext

The HMAC verifies both IV and Ciphertext.

## Authenticated encryption

Authenticated encryption ensures data integrity of the ciphertext. When decrypting, integrity is checked first. Further
decrypting operations will only be executed when the integrity check passes. This prevents certain ciphertext attacks
on AES CBC.

## Requirements

- PHP 5.4 or later with the openssl extension.
- The Defuse PHP-Encryption library from https://github.com/defuse/php-encryption/archive/v2.1.0.zip. Unzip the archive
  and install it as php-encryption in your libraries folder (Example: sites/all/libraries/php-encryption).
- The PHP-Encryption autoload.php file from
  https://gist.github.com/paragonie-scott/949daee819bb7f19c50e5e103170b400/archive/4d72ab0049b1dc37ce68e4cecaf9b280953a1d0a.zip.
  Unzip the archive and place it in the php-encryption directory
  (Example: sites/all/libraries/php-encryption/autoload.php).
- If you are using a version of PHP < 7.0, you will also need to add the random_compat PHP library.
  Download it from https://github.com/paragonie/random_compat/archive/v2.0.11.zip. Install it as random_compat in
  your libraries folder (Example: sites/all/libraries/random_compat). For versions of PHP >= 7.0, this
  library is not needed.

## General configuration

If you need the defuse PHP-encryption library, or use the Encrypt plugin just enable Real AES and install the library.

### Generate a key

To generate a 256-bit random key, use the following command on the Unix CLI:

dd if=/dev/urandom bs=32 count=1 > /path/to/aes.key

This file MUST be stored outside of the docroot. Copy this file to an off-server, safe backup. If you lose the key,
you will not be able to decrypt encrypted information in the database.

If you do not have access to dd, generate the file using drush on a working Drupal installation:

drush php-eval 'echo drupal_random_bytes(32);' > /path/to/aes.key

### Point Real AES to the key

$conf['real_aes_key_file'] = '/path/to/aes.key';

## Encrypt plugin configuration

Real AES adds the "Authenticated AES" encryption method on the "Encryption method settings" tab for Encrypt
configurations.

## Usage

1. Use the Authenticated AES encryption method with the Encrypt module (https://drupal.org/project/encrypt).

2. If you implement encryption yourself, use this module as a Defuse PHP Encryption library loader.
   In your own code, include the library with libraries_load('php-encryption'), then call Crypto::encrypt,
   Crypto::decrypt and Crypto::createNewRandomKey directly.

   See
   * https://github.com/defuse/php-encryption for documentation,
   * For examples, see https://github.com/defuse/encutil or https://github.com/tsusanka/fileencrypt

## Further reading

* Encryption in PHP https://defuse.ca/secure-php-encryption.htm
* Defuse php-encryption readme: https://github.com/defuse/php-encryption/blob/master/README.md
* Authenticated encryption: https://en.wikipedia.org/wiki/Authenticated_encryption
* CBC Block mode: https://en.wikipedia.org/wiki/Block_cipher_mode_of_operation#Cipher_Block_Chaining_.28CBC.29
* HMAC: https://en.wikipedia.org/wiki/Hash-based_message_authentication_code
* SHA-256: https://en.wikipedia.org/wiki/SHA-2

## Key management

Key storage on the webserver is a potential weak point in any encryption system. Consider
using Encrypt with a key management solution, such as Lockr (https://www.drupal.org/project/lockr)
or Townsend Security Key Connection (https://www.drupal.org/project/townsec_key)

## Credits

The original version of this module was created by LimoenGroen - https://limoengroen.nl - after carefully
considering the various encryption modules and libraries.

The library doing the actual work, Defuse PHP-encryption, is authored by Taylor Hornby and Scott Arciszewski. Its
home is https://github.com/defuse/php-encryption .
