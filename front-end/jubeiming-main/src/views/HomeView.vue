<script setup lang="ts">
import RecordComponent from '@/components/RecordComponent.vue'
import GridItemComponent from '@/components/GridItemComponent.vue'
</script>

<template>
  <div id="home-main">
    <div class="container">
      <div class="section">
        <h2>{{ $t('contents.home[0].session') }}</h2>
        <div class="grid">
          <GridItemComponent :title="$t('contents.home[0].grid[0]')" to="#"
            ><i class="fas fa-store"
          /></GridItemComponent>
          <GridItemComponent :title="$t('contents.home[0].grid[1]')" to="#"
            ><i class="fas fa-shopping-bag"
          /></GridItemComponent>
          <GridItemComponent :title="$t('contents.home[0].grid[2]')" to="#"
            ><i class="fas fa-box-open"
          /></GridItemComponent>
          <GridItemComponent :title="$t('contents.home[0].grid[3]')" to="#"
            ><i class="fas fa-camera"
          /></GridItemComponent>
          <GridItemComponent :title="$t('contents.home[0].grid[4]')" to="#"
            ><i class="fas fa-file-alt"
          /></GridItemComponent>
          <GridItemComponent :title="$t('contents.home[0].grid[5]')" to="#"
            ><i class="fas fa-search"
          /></GridItemComponent>
          <GridItemComponent :title="$t('contents.home[0].grid[6]')" to="#"
            ><i class="fas fa-users"
          /></GridItemComponent>
          <GridItemComponent :title="$t('contents.home[0].grid[7]')" to="#"
            ><i class="fas fa-folder"
          /></GridItemComponent>
          <GridItemComponent :title="$t('contents.home[0].grid[8]')" to="#"
            ><i class="fas fa-chart-bar"
          /></GridItemComponent>
          <GridItemComponent :title="$t('contents.home[0].grid[9]')" to="#"
            ><i class="fas fa-chart-line"
          /></GridItemComponent>
        </div>
      </div>
      <div class="section">
        <h2>{{ $t('contents.home[1].session') }}</h2>
        <div class="grid">
          <GridItemComponent :title="$t('contents.home[1].grid[0]')" to="#"
            ><i class="fas fa-gift"
          /></GridItemComponent>
          <GridItemComponent :title="$t('contents.home[1].grid[1]')" to="#"
            ><i class="fas fa-coins"
          /></GridItemComponent>
          <GridItemComponent :title="$t('contents.home[1].grid[2]')" to="#"
            ><i class="fas fa-cut"
          /></GridItemComponent>
          <GridItemComponent :title="$t('contents.home[1].grid[3]')" to="#"
            ><i class="fas fa-bolt"
          /></GridItemComponent>
          <GridItemComponent :title="$t('contents.home[1].grid[4]')" to="#"
            ><i class="fas fa-users"
          /></GridItemComponent>
          <GridItemComponent :title="$t('contents.home[1].grid[5]')" to="#"
            ><i class="fas fa-comments"
          /></GridItemComponent>
          <GridItemComponent :title="$t('contents.home[1].grid[6]')" to="#"
            ><i class="fas fa-calendar-alt"
          /></GridItemComponent>
          <GridItemComponent :title="$t('contents.home[1].grid[7]')" to="#"
            ><i class="fas fa-ticket-alt"
          /></GridItemComponent>
        </div>
      </div>
    </div>
    <RecordComponent bottom="20%"></RecordComponent>
  </div>
</template>

<style scoped>
#home-main {
  position: absolute;
  left: 0;
  right: 0;
}

.container {
  background-color: #fff;
  padding: 20px;
  border-radius: 8px;
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
}

.section {
  margin-bottom: 20px;
}

.section h2 {
  margin-bottom: 10px;
  font-weight: bolder;
}

.grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(100px, 1fr));
  gap: 15px;
}
</style>
