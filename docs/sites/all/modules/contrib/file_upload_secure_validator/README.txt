This is a very simple and small module which performs a server side validation
for the extension of an uploaded file of any content type's file field. Default
drupal 7 file validation is performed by file_validate_extensions which only
relies on the file name extension.
'File Upload Secure Validator' uses the php library 'fileinfo' and is dependent
on that. Therefore the server hosting the drupal instance should have this
library enabled. Through this php lib we can perform a more secure and reliable
check on the file's mime type and compare that to the allowed file extensions,
as these are set by the admin within the content type's field settings.

This module is useful when we need to enforce a maximum security mime type
detection.

Dependencies
The module depends on the php library <strong>fileinfo</strong>. Please make
sure this library is present and enabled on the server.

Installation
Install module like usual. No special installation considerations

Configuration 
No configuration options. After enabling the module, it will perform an
alternative server side extension validation on every uploaded file of every
content type file field.
