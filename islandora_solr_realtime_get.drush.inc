<?php
/**
 * @file
 * Functions for retrieve solr document via drush.
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
 * Implements hook_drush_command().
 */
function islandora_solr_realtime_get_drush_command() {
  $items['islandora_solr_realtime_get'] = array(
    'description' => 'Retrieve a Solr document using the RealTime Get interface.',
    'options' => array(
      'id' => 'The ID of the Solr document.',
      'ids' => 'The IDs of the Solr documents, separated by comma.',
      'fields' => 'The fields to retrieve, separated by comma. If not defined, retrieve all available fields.',
      'time' => 'Time how long it takes to retrieve id(s), can take a number to repeat.',
    ),
    'aliases' => array('isrtg', 'realtimeget'),
    'examples' => array(
      'drush --user=admin islandora_solr_realtime_get --id=islandora:1',
      'drush --user=admin realtimeget --id=islandora:2',
    ),
  );
  return $items;
}

/**
 * Implements drush_hook_COMMAND_validate().
 */
function drush_islandora_solr_realtime_get_validate() {
  $id = drush_get_option('id');
  $ids = drush_get_option('ids');
  $time = drush_get_option('time');
  if (!isset($id) && !isset($ids)) {
    return drush_set_error("Argument id(s) missing");
  }
  if (isset($time) && !is_numeric($time)) {
    return drush_set_error("Argument time should be a number");
  }
}

/**
 * Implements drush_hook_COMMAND().
 */
function drush_islandora_solr_realtime_get() {
  module_load_include('inc', 'islandora_solr_realtime_get', 'includes/utilities');

  $id = drush_get_option('id');
  $ids = drush_get_option('ids');
  $fields = drush_get_option('fields');
  $time = drush_get_option('time');

  if (isset($fields)) {
    $fields = explode(',', $fields);
  }
  if (isset($ids)) {
    $ids = explode(',', $ids);
    if (isset($id)) {
      $ids[] = $id;
    }
  }
  elseif (isset($id)) {
    $ids = array($id);
  }
  if (!isset($time) || ($time + 0) <= 0) {
    foreach ($ids as $id) {
      print ">>>>> Record for $id:\n";
      islandora_solr_realtime_get_retrieve_and_print($id, $fields);
    }
  }
  else {
    // RealTime Get:
    $time = $time + 0;
    $start = microtime(TRUE);
    $recordcount = 0;
    $fieldcount = 0;
    for ($i = 0; $i < $time; $i++) {
      foreach ($ids as $id) {
        $result = islandora_solr_realtime_get_retrieve_array($id, $fields);
        if ($result) {
          $recordcount += 1;
          $fieldcount += count($result);
        }
      }
    }
    $end = microtime(TRUE);
    print "RealTime Get: $recordcount records ($fieldcount fields) retrieved in " . sprintf("%.6f", ($end - $start)) . " seconds\n";

    // Solr Search with q:
    if ( module_load_include('inc', 'islandora_solr', 'includes/query_processor')) {
      $time = $time + 0;
      $start = microtime(TRUE);
      $recordcount = 0;
      $fieldcount = 0;
      for ($i = 0; $i < $time; $i++) {
        foreach ($ids as $id) {
          $qp = new IslandoraSolrQueryProcessor();
          $query = "PID:" . islandora_solr_lesser_escape($id);
          $qp->buildQuery($query);
          $qp->solrParams['fq'] = array(); // make sure we get all the results
          if (isset($fields)) {
            $qp->solrParams['fl'] = $fields;
          }
          $qp->executeQuery(false);
          $r = $qp->islandoraSolrResult;
          if ($r['response']['numFound'] === 1) {
            $solrdoc = $r['response']['objects'][0]['solr_doc'];
            if ($solrdoc) {
              $recordcount += 1;
              $fieldcount += count($solrdoc);
            }
          }
        }
      }
      $end = microtime(TRUE);
      print "Solr search with q: $recordcount records ($fieldcount fields) retrieved in " . sprintf("%.6f", ($end - $start)) . " seconds\n";

      // Solr Search with fq:
      $time = $time + 0;
      $start = microtime(TRUE);
      $recordcount = 0;
      $fieldcount = 0;
      for ($i = 0; $i < $time; $i++) {
        foreach ($ids as $id) {
          $qp = new IslandoraSolrQueryProcessor();
          $query = "PID:" . islandora_solr_lesser_escape($id);
          $qp->buildQuery('*:*');
          $qp->solrParams['fq'] = array($query); // make sure we get all the results
          if (isset($fields)) {
            $qp->solrParams['fl'] = $fields;
          }
          $qp->executeQuery(false);
          $r = $qp->islandoraSolrResult;
          if ($r['response']['numFound'] === 1) {
            $solrdoc = $r['response']['objects'][0]['solr_doc'];
            if ($solrdoc) {
              $recordcount += 1;
              $fieldcount += count($solrdoc);
            }
          }
        }
      }
      $end = microtime(TRUE);
      print "Solr search with fq: $recordcount records ($fieldcount fields) retrieved in " . sprintf("%.6f", ($end - $start)) . " seconds\n";
    }
  }
}

function islandora_solr_realtime_get_retrieve_and_print($id, $fields = NULL) {
  module_load_include('module', 'islandora_solr_realtime_get');
  $result = islandora_solr_realtime_get_retrieve_array($id, $fields);

  if ($result) {
    foreach ($result as $field => $value) {
      if (is_array($value)) {
        print "$field => [ " . implode(",", $value) . " ]\n";
      }
      else {
        print "$field => $value\n";
      }
    }
  }
  else {
    print "No solr record found for $id\n";
  }
}
