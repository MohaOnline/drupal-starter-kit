# File MD5

Module helps to uniquely identify the files using their MD5 hashes.

All that this module does - extends the `file_managed` table with `md5` column and, on file saving, set a value for it.

## MD5 validation

Module provides an implementation of `hook_file_validate()` to validate file duplicates. This validation could be disabled here: `admin/config/media/file-system`.

## Example implementation

- Create a form with submit callback.
- Save MD5 hash instead of file ID.
- Export configuration via [Features](https://drupal.org/project/features).
- Add files to version control system.
- Reinstall the site and add files into database.

All these steps implemented in a test module: [File MD5 (Test)](tests/file_md5_test).

**Note**: likely, this module will not be understandable for newbies. Use it if you'd really know what to do.
