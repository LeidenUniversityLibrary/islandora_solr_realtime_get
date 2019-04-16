<?php

/**
 * @file
 * includes/admin.form.inc
 *
 *
 *  Copyright 2019 Leiden University Library
 *
 *  This file is part of islandora_solr_realtime_get.
 *
 *  islandora_solr_realtime_get is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

function islandora_solr_realtime_get_view(AbstractObject $object) {
  module_load_include('inc', 'islandora_solr_realtime_get', 'includes/utilities');
  $json = islandora_solr_realtime_get_retrieve_json($object->id);
  if (isset($json)) {
    return array(
      'solr' => array(
        '#prefix' => '<pre>',
        '#markup' => json_encode(json_decode($json), JSON_PRETTY_PRINT),
        '#suffix' => '</pre>',
      ),
    );
  }
  else {
    return array(
      'solr' => array(
        '#prefix' => '<p>',
        '#markup' => t('No Solr document found.'),
        '#suffix' => '</p>',
      ),
    );
  }
}
