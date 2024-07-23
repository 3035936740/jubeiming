import { createRouter, createWebHistory } from 'vue-router'
import HomeView from '@/views/HomeView.vue'
import MeView from '@/views/MeView.vue'
import useNavbarFocus from '@/stores/navbarfocus'
import { storeToRefs } from 'pinia'
import { ref } from 'vue'
import MessagesView from '@/views/MessagesView.vue'
import OrdersView from '@/views/OrdersView.vue'
// import DevView from '@/views/DevView.vue'

import {core} from "@/api";
import useUserRecord from '@/stores/userRecord'

const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes: [
    {
      path: '/',
      name: 'home',
      component: HomeView,
      beforeEnter: (to, from, next) => {
        const navbarFocus = useNavbarFocus()
        storeToRefs(navbarFocus).elementNumber = ref(0)
        next()
      }
    },
    {
      path: '/orders',
      name: 'orders',
      component: OrdersView,
      beforeEnter: (to, from, next) => {
        const navbarFocus = useNavbarFocus()
        storeToRefs(navbarFocus).elementNumber = ref(1)
        next()
      }
    },
    {
      path: '/message',
      name: 'message',
      component: MessagesView,
      beforeEnter: (to, from, next) => {
        const navbarFocus = useNavbarFocus()
        storeToRefs(navbarFocus).elementNumber = ref(2)
        next()
      }
    },
    {
      path: '/me',
      name: 'me',
      component: MeView,
      /*
       * to: 表示即将进入的目标路由对象。它包含有关目标路由的各种信息，如路由路径、名称、参数、元数据等。
       * from: 表示当前导航正要离开的路由对象。类似于 to，它包含有关当前路由的各种信息。
       * next: 是一个函数，用于控制导航流程。在导航守卫中，您必须调用 next 函数来进入下一个钩子。如果不调用 next()，导航将会被中断。
       * */

      beforeEnter: (to, from, next) => {
        const navbarFocus = useNavbarFocus()
        storeToRefs(navbarFocus).elementNumber = ref(3)
        // 调用初始化函数
        next()
        /*
         * next(): 继续导航。
         * next(false): 中断导航。
         * next('/path'): 跳转到指定路径。
         * next(error): 中断导航，并且把错误传递给 router.onError() 注册的回调函数。
         * */
      }
    }
  ]
})

router.beforeEach(async(to, from, next) => {
  // 在每次路由变更前调用的函数
  globalFunction();
  next(); // 继续导航
});

async function globalFunction() {
  console.log('This function is called on every route change or page refresh.');
  const userRecord = useUserRecord()

  let {access_token} = userRecord.accessToken

  let me = (await core.me(access_token)).data;
  if (!me) {
    const {refresh_token} = userRecord.refreshToken
    const data = (await core.freshAccessToken(refresh_token)).data;
    if (!data){
      userRecord.setAccessToken("", 0)
      userRecord.setRefreshToken("", 0)
      return;
    }
    access_token = data.access_token
    const expires_in = data.expires_in
    userRecord.setAccessToken(access_token, expires_in)
    me = (await core.me(access_token)).data;
  }
  userRecord.setMe(me);


}

export default router
