# Glazed Theme

Framework Theme that is central to SooperThemes products. All designs for sale on the site are using Glazed theme, with customized content and theme settings. For a quick live demo of the product visit http://www.trysooperthemes.com/

For testing and development it is preferred that your installation is based on the Main Demo installation profile. You can download it here: https://www.sooperthemes.com/download

## Workflow

* Develop and Test locally on your own machine
* Push  code to a development, feature or issue branch. Create pull request.
* I will test and give feedback
* If code is OK I will merge with the 7.x branch.
* Branches should be named 7.x-yourname-branchname

Branch naming examples:
```
7.x-jur-dev
```
```
7.x-jur-issue_777789
```
```
7.x-jur-typography_settings_googlefonts
```


### Prerequisites

* [Drupal 7](https://www.drupal.org/project/drupal)
* [Bootstrap basetheme](https://www.drupal.org/project/bootstrap)
* [jQuery Update](https://www.drupal.org/project/jquery_update) - Set to load 2.1 on frontend pages and 1.8 on seven theme)

### Installing

Installs like any other theme. Recommended: install glazed_free_helper (Glazed Theme Tools) module.

```
drush en glazed_free_builder -y
```

To enable the module on a field, set Glazed Builder formatter on any entity textfield. For example on Basic Page body field (example.com/admin/structure/types/manage/page/display). The builder should show when viewing a node of this type (not on de node/add or node/edit form).

### Developing

[Grunt](http://gruntjs.com/) is used to parse sass to CSS and combine JS files. Don't run npm install, development modules are included.

```
grunt
```

## Built With

* [Drupal 7](https://www.drupal.org/project/drupal) - The web framework used
* [jQuery](https://jquery.com/) - JS Framework
* [underscore](http://underscorejs.org/) - JS Tools
* [jQuery UI](https://jqueryui.com/) - Drag and Drop
* [Grunt](http://gruntjs.com/) - Combining JS and CSS files, adding CSS prefixes

## Versioning

This project follows [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/jjroelofs/glazed/tags).

### Code Standards and Best Practices

* https://trello.com/b/LdWR68Cm/sooperthemes-drupal-wiki
* https://www.drupal.org/coding-standards
