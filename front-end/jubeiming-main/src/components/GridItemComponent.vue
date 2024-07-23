<script setup lang="ts"></script>

<template>
  <RouterLink :to="to" class="nav-item">
    <div class="grid-item">
      <div class="icon">
        <slot></slot>
      </div>
      {{ title }}
    </div>
  </RouterLink>
</template>

<script lang="ts">
export default {
  name: 'GridItemComponent',
  props: {
    title: {
      type: String,
      required: true
    },
    to: {
      type: String,
      required: false,
      default: '#'
    }
  }
}
</script>

<style scoped>
.nav-item {
  display: contents;
  text-decoration: none;
  color: black;
  padding: 1px;
  flex: 1;
  transition: background-color 0.3s ease;
}

.grid-item {
  background: #f5f5f5;
  padding: 20px;
  text-align: center;
  border-radius: 10px;
  box-shadow: -1px 1px 10px rgba(0, 0, 0, 0.15);
  transition: background 0.3s ease;
}

.grid-item:hover {
  background: #e0e0e0;
}

.grid-item:active{
  background-color: #a0a0a0;
}

.icon {
  font-size: 24px;
  margin-bottom: 10px;
}
</style>
