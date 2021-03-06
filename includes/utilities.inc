<?php

/**
 * @file
 * islandora_solr_realtime_get.module
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

/**
 * Retrieves the (requested) fields of a Solr document identified by $pid.
 *
 * @return output
 *   An associative array with the (requested) fields of the found Solr
 *   document as keys and their values as value (multivalued fields are
 *   represented as indexed arrays, singlevalued fields as string.
 *   Returns NULL when no Solr document was found for given $pid.
 */
function islandora_solr_realtime_get_retrieve_json($pid, $fields = NULL) {
  if (module_load_include('inc', 'islandora_solr', 'includes/luke')) {
    $solr_url = variable_get('islandora_solr_url', 'localhost:8080/solr');
    if (islandora_solr_ping($solr_url)) {
      $realtime_get_url = islandora_solr_check_http($solr_url . '/get');
      $query = array('wt' => 'json', 'id' => $pid);
      if ($fields !== NULL) {
        $query['fl'] = implode(',', $fields);
      }
      $realtime_get_url = url($realtime_get_url, array('absolute' => TRUE, 'query' => $query));
      $response = drupal_http_request($realtime_get_url);
      if ($response->code >= 200 && $response->code < 300) {
        return $response->data;
      }
    }
  }
  return NULL;
}

function islandora_solr_realtime_get_retrieve_array($pid, $fields = NULL) {
  $json = islandora_solr_realtime_get_retrieve_json($pid, $fields);
  if (isset($json)) {
    $get_array = json_decode($json, TRUE);
    if (isset($get_array['doc'])) {
      return $get_array['doc'];
    }
  }
  return NULL;
}
