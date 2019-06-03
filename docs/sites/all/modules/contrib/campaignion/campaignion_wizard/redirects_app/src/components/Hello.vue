<template>
  <div class="hello">
    <h1>{{ msg }}</h1>
    <h2>Submit handling</h2>
    <div v-if="askingToLeave">
      May I leave?
      <button type="button" @click="leave">Go for it!</button>
      <button type="button" @click="stay">No way!</button>
    </div>
    <h2>Test vuex</h2>
    Count: {{ count }} <a href="#" @click="add10">add 10</a>
    <h2>This instance lives in {{ containerId }}</h2>
  </div>
</template>

<script>
import {dispatch} from '@/utils'

export default {
  name: 'hello',
  data () {
    return {
      msg: 'Welcome to Your Vue.js App',
      askingToLeave: false
    }
  },
  computed: {
    count () {
      return this.$store.state.count
    },
    containerId () {
      return this.$root.$options.drupalContainer.id
    }
  },
  mounted () {
    const listener = e => {
      if (e.type === 'request-leave-page') {
        // User wants to go back - ask: lose data?
      } else {
        // User wants to submit: valildate and save data.
      }
      this.askingToLeave = true
    }
    this.$root.$el.addEventListener('request-submit-page', listener)
    this.$root.$el.addEventListener('request-leave-page', listener)
  },
  methods: {
    add10 () {
      this.$store.dispatch({
        type: 'incrementAsync',
        amount: 10
      })
    },
    leave () {
      this.askingToLeave = false
      dispatch(this.$root.$el, 'resume-leave-page')
    },
    stay () {
      this.askingToLeave = false
      dispatch(this.$root.$el, 'cancel-leave-page')
    }
  }
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
h1, h2 {
  font-weight: normal;
}

ul {
  list-style-type: none;
  padding: 0;
}

li {
  display: inline-block;
  margin: 0 10px;
}

a {
  color: #42b983;
}
</style>
