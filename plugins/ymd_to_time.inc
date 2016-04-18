<?php

$plugin = array(
  'form'     => 'ymd_tamper_form',
  'callback' => 'ymd_tamper_callback',
  'name'     => 'Date in Ymd format to Y-m-d h:i:s or time',
  'multi'    => 'direct',
  'category' => 'Other',
);

function ymd_tamper_form($importer, $element_key, $settings) {
  $form = array();

  $form['instructions'] = array(
    '#markup' => t('Expected input: a string with date in format Ymd: 20161201')
  );
  
  $form['to_time'] = array(
    '#type' => 'select',
    '#title' => t('Convert date string to time') . '?',
    '#options' => array(
        0 => t('Yes'),
        1 => t('No'),
    ),
    '#default_value' => isset($settings['to_time']) ? $settings['to_time'] : '',
    '#description' => t('You always need to return time (this option was made to avoid an strange database error)'),
  );

  return $form;
}

function ymd_tamper_callback($source, $item_key, $element_key, &$field, $settings) {
  if (!is_array($field)) {
    $field = array($field);
  }

  $out = array();

  foreach ($field as $f) {
    if(is_array($f)) {
      foreach ($f as $f_values) {
        $f_values = ymd_tamper_process($f_values);
        $f_values = (isset($settings['to_time']) && $settings['to_time']) ? strtotime($f_values) : $f_values;
        $out[] = $f_values;
      }
    } else {
      $f = ymd_tamper_process($f);
      $f = (isset($settings['to_time']) && $settings['to_time']) ? strtotime($f) : $f;
      $out[] = $f;
    }
  }
  if (count($out) == 1) {
    $out = $out[0];
  }
  $field = $out;
}

/**
 * Converts Ymd data format to Y-m-d H:i:s format.
 */
function ymd_tamper_process($date) {
  $y = substr($date, 0, 4);
  $m = substr($date, 4, 2);
  $d = substr($date, 6, 4);

  return $y . '-' . $m . '-' . $d;
}
