<template>
  <div class="icon-wrapper">
    <RouterLink :to="to" class="nav-item">
      <div class="icon-container">
        <i :class="['fas', icon, 'icon-item']" :style="'font-size: ' + size"></i>
        <span>{{ title }}</span>
      </div>
    </RouterLink>
    <div>
      <slot></slot>
    </div>
  </div>
</template>

<script lang="ts">
export default {
  name: 'MessageBarItem',
  props: {
    title: String,
    icon: String,
    size: {
      type: String,
      default: '18px'
    },
    to: {
      type: String,
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
}

.icon-wrapper {
  position: relative;
  display: flex;
  align-items: center;
  justify-content: center;
}

.icon-container {
  display: flex;
  align-items: center;
}

.icon-container:hover .icon-item,
.icon-container:hover span {
  color: #328fb9;
}

.icon-container:active .icon-item,
.icon-container:active span {
  color: #17b6ff;
}

.icon-container span,
.icon-container .icon-item {
  margin-left: 4px;
  font-size: v-bind(size);
}
</style>
