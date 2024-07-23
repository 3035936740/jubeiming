import { defineStore } from 'pinia'
import { ref } from 'vue'

const useSelectOrderId = defineStore(
    'select-order-id',
    () => {
        const order_id = ref<number>(0)
        const show_drawer = ref<boolean>(false)

        const setOrderID = (_order_id: number) => {
            order_id.value = _order_id
        }

        return {
            order_id,
            setOrderID,
            show_drawer
        }
    }
)

export default useSelectOrderId
