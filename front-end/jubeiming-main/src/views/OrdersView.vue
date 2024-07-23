<script setup lang="ts">
import RecordComponent from '@/components/RecordComponent.vue'
import Orders from '@/components/orders/OrdersComponent.vue'
import Detail from '@/components/orders/DetailComponent.vue'
import useSelectOrderId from '@/stores/selectOrderId'

import {ref} from "vue";
import {storeToRefs} from "pinia";

const ordersInfo = ref([
  ["新歇",96135,0],
  ["太漂亮了",92977,3],
  ["哎呦",16976,2],
  ["难搞",981367,1],
])

const SelectOrderId = useSelectOrderId()
const { show_drawer, order_id } = storeToRefs(SelectOrderId)
</script>

<template>
  <div id="orders-main">
    <div class="container">
      <div class="section">
        <h2>{{ $t('contents.orders.session_name') }}</h2>

        <Orders :ordersInfo="ordersInfo"></Orders>
      </div>
    </div>
    <RecordComponent></RecordComponent>
  </div>


  <el-drawer
      direction="btt"
      v-model="show_drawer"
      title="I am the title"
      :with-header="false"
      size="100%"
      class="detail-drawer"
  >
    <div class="drawer-top">
      <span class="touch left" @click="show_drawer = false"><i class="fas fa-chevron-left"/></span>
      <span class="touch right" @click="null"><i class="fas fa-ellipsis-v" /></span>
    </div>

    <span class="trading-status">
      <div class="status-item">
      完成交易
      </div>
    </span>

    <Detail>
    </Detail>
  </el-drawer>
</template>

<script lang="ts">
export default {
  name: "OrdersView"
}
</script>

<style scoped>
.detail-drawer{
  background-color: #a0a0a0;
}

.trading-status {
  display: flex;
  justify-content: flex-start;
  align-items: center;
  padding: 20px 10px;
  margin-right: 30px;
}

.trading-status .status-item{
  font-size: 30px;
  padding-right: 10px;
  font-weight: bold;
}

.drawer-top {
  background-color: #ececec;
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 15px;
  border-radius: 8px;
  font-size: 20px;
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
}

.drawer-top .touch {
  cursor: pointer;
  color: #9c9c9c;
}

.drawer-top .left{
  padding: 0 40px 0 10px;
}

.drawer-top .right {
  padding: 0 10px 0 40px;
}

.drawer-top .touch:active {
  color: #c5c5c5;
}

#orders-main {
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
  padding-top: 10px;
}
</style>