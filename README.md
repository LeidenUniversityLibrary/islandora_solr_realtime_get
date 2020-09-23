# Islandora Solr Realtime Get 

# Introduction

Islandora Solr Realtime Get is a module to retrieve a document from Solr in realtime.

## Requirements

This module requires the following modules/libraries:

* [Islandora Solr](https://github.com/islandora/islandora_solr_search)

## Installation
 
Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.
 
## Configuration

No further configuration is needed to use this module.

## Documentation

This module provides an extra tab for an Islandora object that displays the Solr document.

Via drush one or more Solr documents can be retrieved:

```drush --user=admin islandora_solr_realtime_get --id=islandora:1```
Retrieves full Solr document for object islandora:1.

```drush --user=admin realtimeget --ids=islandora:1,islandora:2```
Retrieves full Solr documents for objects islandora:1 and islandora:2.

```drush --user=admin realtimeget --ids=islandora:1,islandora:2 --fields=PID,timestamp```
Retrieves only fields PID and timestamp for objects islandora:1 and islandora:2.

```drush --user=admin realtimeget --ids=islandora:1,islandora:2 --time=10```
Retrieves 10 times full Solr documents for objects islandora:1 and islandora:2, and displays how long it took.

## Maintainers/Sponsors

Current maintainers:

* [Lucas van Schaik](https://github.com/lucasvanschaik)

## Development

If you would like to contribute to this module, please contact the current maintainer.

## License

[GPLv3](LICENSE.txt)
Copyright 2019-2020 Leiden University Library
