if (!self.AIOne_BASE_URL) {
    throw new Error("AIOne_BASE_URL was not configured");
}

importScripts(
    new URL("AIOneWeb.js", self.AIOne_BASE_URL).href,
    new URL("worker-runtime.js", self.AIOne_BASE_URL).href
);