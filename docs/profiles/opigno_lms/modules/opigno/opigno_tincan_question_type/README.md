# Opigno TinCan Question Type
## Synopsis

This module adds a new question type for the [Quiz module](https://www.drupal.org/project/quiz).
With this new question type, you will be able to import TinCan Packages to your Drupal instance and to use it as a
question.

This module as been created mainly for the [Opigno LMS](https://www.drupal.org/project/opigno_lms) and is maintained by
[Connect-i](https://www.drupal.org/node/1773678), the editor of Opigno.


## Requirements

This module uses functions that are provided by these modules:
- [Opigno TinCan API](https://www.drupal.org/sandbox/amermod/2696181)
- [Quiz](https://www.drupal.org/project/quiz)
- Quiz Question (included in the Quiz module)

This module also needs the [TinCanPHP](https://github.com/RusticiSoftware/TinCanPHP) library in order to function well.
Install the library inside the folder *sites/all/libraries/*. The library folder should be named **TinCanPHP**.


## Dependencies

In order to be functional, this module will need a Learning Record Store (LRS). You can find one on
[this page](https://tincanapi.com/get-lrs/).


## Minimum requirements for the TinCan Packages

### About *tincan.xml*
- The TinCan Package must contain the file *tincan.xml*.
- This file must follow the guidelines written [here](https://github.com/RusticiSoftware/launch/blob/master/lms_lrs.md).
- This file must contain, at least, an Activity with an Activity ID and the launch file.

So, the file should be, at least, like this one:
```xml
<?xml version="1.0" encoding="utf-8" ?>
<tincan xmlns="http://projecttincan.com/tincan.xsd">
    <activities>
        <activity id="http://example.com/my-activity-id">
            <launch>index.html</launch>
        </activity>
    </activities>
</tincan>
```

### About statement
- The package should send a statement to the LRS containing the final score.
- This statement must use the verb *http://adlnet.gov/expapi/verbs/passed* or *http://adlnet.gov/expapi/verbs/failed*.
- The score property in this statement should have, at least, the *scaled* property or the *raw* and *max* properties or
   the *success* property.
- This statement must use the *Activity ID* declared in the *tincan.xml* file.
- The statement must use the *registration UUID* given in parameter of the launch file.

So, the statement should look, at least, like this:
```json
{
    "actor": {
        "objectType": "Agent",
        "mbox_sha1sum": "3e13b53bf292605d5267223fa8ba78ec27402401",
        "name": "admin"
    },
    "verb": {
        "id": "http://adlnet.gov/expapi/verbs/passed",
        "display": {
            "en-US": "failed"
        }
    },
    "object": {
        "objectType": "Activity",
        "id": "http://example.com/my-activity-id"
    },
    "result": {
        "score": {
            "scaled": 0.33
        }
    },
    "context": {
        "registration": "21ee665f-7111-4324-b92c-d31ebf02b0f4"
    }
}
```
