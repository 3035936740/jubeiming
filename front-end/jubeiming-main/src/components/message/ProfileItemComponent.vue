<script setup lang="ts">
</script>

<template>
  <div class="profile-item">
    <router-link :to="to" class="nav-item">
      <div class="router-item">
      <div class="profile-avatar">
        <img :src="src" alt="Avatar" />
        <span class="profile-name">{{ username }}</span>
      </div>
      </div>
    </router-link>
    <div class="end-ndc">
      <slot></slot>
    </div>
  </div>
</template>

<script lang="ts">
export default {
  name: 'ProfileItem',
  props: {
    username: {
      type: String,
      default: ""
    },
    src: {
      type: String,
      default: "https://via.placeholder.com/40"
    },
    to: {
      type: String,
      default: "#"
    }
  }
}
</script>

<style scoped>
.end-ndc {
  position: relative;
  display: flex;
  align-items: center;
  margin: 10px;
}
.nav-item {
  display: contents;
  text-decoration: none;
  color: black;
}

.router-item {
  display: flex;
  flex-grow: 1;
}

.profile-item {
  display: flex;
  align-items: center;
  justify-content: space-between;
  border-bottom: 1px solid #ddd;
}

.profile-avatar {
  display: flex;
  align-items: center;
  flex-grow: 1;
  padding: 10px 0;
}

.profile-avatar img {
  border-radius: 50%;
  width: 40px;
  height: 40px;
  margin: 0 10px;
}

.profile-name {
  margin-left: 10px;
  color: black;
  white-space: nowrap;
  overflow: hidden;
  text-overflow: ellipsis;
  font-size: 18px;
}
</style>
