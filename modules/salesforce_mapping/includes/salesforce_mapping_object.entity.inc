<?php

/**
 * @file
 * Contains SalesforceMappingObject.
 */

/**
 * Entity class for Salesforce Mapping Objects
 */
class SalesforceMappingObject extends Entity {

  // @codingStandardsIgnoreStart
  public
    $salesforce_mapping_object_id,
    $revision_id,
    $salesforce_id,
    $entity_id,
    $entity_type,
    $created,
    $entity_updated,
    $last_sync,
    $last_sync_action,
    $last_sync_status,
    $last_sync_message;
  // @codingStandardsIgnoreEnd

  /**
   * Constructor for SalesforceMappingObject.
   *
   * @param array $values
   *   Associated array of values this entity should start with.
   */
  public function __construct(array $values = array(), $entityType = NULL) {
    parent::__construct($values, 'salesforce_mapping_object');
  }

  public function id() {
    return $this->salesforce_mapping_object_id;
  }

  public function entityType() {
    return 'salesforce_mapping_object';
  }

  public function label() {
    if (isset($this->is_new) && $this->is_new === TRUE) {
      return '';
    }
    $label = NULL;
    $wrapper = entity_metadata_wrapper('salesforce_mapping_object', $this);
    $entity = $wrapper->entity->value();
    if ($entity) {
      $label = $entity->label();
    }
    if (empty($label)) {
      $label = $this->entity_type . ':' . $this->entity_id;
    }
    return t('"@label" to "@sf_id"', array('@label' => $label, '@sf_id' => $this->salesforce_id));
  }

  public function uri() {
    $uri = NULL;
    $wrapper = entity_metadata_wrapper($this->entityType(), $this);
    $entity = $wrapper->entity->value();
    if ($entity) {
      $uri = $entity->uri();
    }
    if (empty($uri)) {
      $path = 'admin/content/salesforce/' . $this->entity_type . '/' . $this->entity_id;
    }
    else {
      $path = $uri['path'];
    }
    return array(
      'path' => $path . '/salesforce_activity',
      'options' => array(),
    );
  }

  /**
   * Save the entity.
   *
   * @return object
   *   The newly saved version of the entity.
   */
  public function save() {
    if (isset($this->is_new) && $this->is_new) {
      $this->created = REQUEST_TIME;
    }

    if (!isset($this->is_new_revision)) {
      $this->is_new_revision = TRUE;
    }

    if (!isset($this->default_revision)) {
      $this->default_revision = TRUE;
    }

    return parent::save();
  }
}
