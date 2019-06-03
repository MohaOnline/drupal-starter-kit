export default {
  incrementAsync (context, {amount}) {
    setTimeout(() => {
      context.commit('increment', {amount})
    }, 1000)
  }
}
