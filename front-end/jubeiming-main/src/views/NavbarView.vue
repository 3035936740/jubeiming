<script setup lang="ts">
import NavbarRouterComponent from '@/components/NavbarRouterComponent.vue'
import useNavbarFocus from '@/stores/navbarfocus'
import { storeToRefs } from 'pinia'

const selectedIndex = storeToRefs(useNavbarFocus()).elementNumber

const handleSelect = (index: number) => {
  selectedIndex.value = index
}

handleSelect(selectedIndex.value)
</script>

<template>
  <nav class="navbar">
    <NavbarRouterComponent
      to="/"
      icon="fa-home"
      :is-selected="selectedIndex === 0"
      @select="() => handleSelect(0)"
      :index="0"
    >
      {{ $t('contents.navbar[0]') }}
    </NavbarRouterComponent>
    <NavbarRouterComponent
      to="/orders"
      icon="fa-box"
      :is-selected="selectedIndex === 1"
      @select="() => handleSelect(1)"
      :index="1"
    >
      {{ $t('contents.navbar[1]') }}
    </NavbarRouterComponent>
    <NavbarRouterComponent
      to="/message"
      icon="fa-comments"
      :is-selected="selectedIndex === 2"
      @select="() => handleSelect(2)"
      :index="2"
    >
      {{ $t('contents.navbar[2]') }}
    </NavbarRouterComponent>
    <NavbarRouterComponent
      to="/me"
      icon="fa-user"
      :is-selected="selectedIndex === 3"
      @select="() => handleSelect(3)"
      :index="3"
    >
      {{ $t('contents.navbar[3]') }}
    </NavbarRouterComponent>
  </nav>
</template>

<style scoped>
.navbar {
  background-color: #333;
  display: flex;
  justify-content: space-around;
  align-items: center;
  position: fixed;
  bottom: 0;
  width: 100%;
  z-index: 1000; /* Ensure it stays above other content */
}
</style>
