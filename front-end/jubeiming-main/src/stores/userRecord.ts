import { defineStore } from 'pinia'

const useUserRecord = defineStore('user-record', {
        state: () => {
            return {
                access_token: null as string,
                access_token_expires_in: null as number,
                refresh_token: null as string,
                refresh_token_expires_in: null as number,
                me: null
            }
        },
        getters: {
            accessToken(state) {
                return {
                    access_token: state.access_token,
                    expires_in: state.access_token_expires_in
                }
            },
            refreshToken(state) {
                return {
                    refresh_token: state.refresh_token,
                    expires_in: state.refresh_token_expires_in
                }
            },
            Me(state) {
                return state.me
            }
        },
        actions: {
            setAccessToken(accessToken : string, expiresIN: number){
                this.access_token = accessToken
                this.access_token_expires_in = expiresIN
            },
            setRefreshToken(refreshToken : string, expiresIN: number){
                this.refresh_token = refreshToken
                this.refresh_token_expires_in = expiresIN
            },
            setMe(me){
                this.me = me
            },
        },
        persist: {
            storage: localStorage
        }
    }
)

export default useUserRecord
