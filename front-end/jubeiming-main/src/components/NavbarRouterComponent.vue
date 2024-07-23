<script setup lang="ts">
import { defineProps, defineEmits, onMounted } from 'vue'

const props = defineProps<{
  to: string
  icon: string
  isSelected: boolean
  index: number
}>()

const emit = defineEmits(['select'])

onMounted(() => {
  const allElements = document.querySelectorAll('.nav-item')
  const routerElement = allElements[props.index] as HTMLAnchorElement
  if (props.isSelected && routerElement) {
    routerElement.focus()
    //if (routerElement)routerElement.click()
  }
})
</script>

<template>
  <RouterLink
    :to="to"
    ref="router"
    :class="['nav-item', { selected: isSelected }]"
    @click.prevent="emit('select')"
  >
    <div class="icon-container">
      <i :class="'icon fas ' + icon" />
    </div>
    <div class="text"><slot></slot></div>
  </RouterLink>
</template>

<style scoped>
.nav-item {
  text-align: center;
  text-decoration: none;
  color: #fff;
  padding: 10px;
  flex: 1;
  transition: background-color 0.3s ease;
  outline: none;
}

.nav-item:hover {
  background-color: #555;
}
.icon-container {
  font-size: 24px;
}
.icon-container i {
  display: block;
}
.icon-container i::before {
  margin: 0 auto; /* Center the icon */
}
.text {
  display: block;
  font-size: 16px;
  margin-top: 2px;
  font-weight: lighter;
}
.selected .icon,
.selected .text {
  color: #449df8;
}

.nav-item:hover .icon {
  color: #a4a4a4;
}

.nav-item:hover .text {
  color: #dadada;
}

.nav-item:focus .icon,
.nav-item:focus .text {
  color: #449df8;
}
</style>
