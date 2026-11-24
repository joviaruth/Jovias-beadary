# Jovia's Beadary

A responsive marketing storefront for Jovia's handcrafted beadwork business, with an optional C++ inquiry API.

## Preview locally

The frontend has no build step:

```bash
npx serve .
```

Open the URL printed in the terminal. Without an API URL, the inquiry form safely opens a prepared email to Jovia.

## Frontend deployment

- **Vercel:** import this repository and deploy with framework preset “Other.” No build command is required.
- **Netlify:** import the repository. `netlify.toml` already sets the publish directory.
- **GitHub Pages:** enable Pages for the `main` branch and repository root.

Before publishing, replace the placeholder WhatsApp number and social links in `index.html`.

## C++ API

The API uses C++17 and Crow. It accepts inquiries at `POST /api/inquiries` and exposes `GET /api/health`.

```bash
cd backend
cmake -S . -B build
cmake --build build
./build/jovias-api
```

For a hosted backend, deploy `backend/Dockerfile` to a container provider such as Render, Railway, Fly.io, or Google Cloud Run. Configure:

- `PORT` — service port (defaults to `8080`)
- `ALLOWED_ORIGIN` — deployed frontend URL

Then define `window.JOVIA_API_URL` before `script.js` in `index.html`, pointing to the deployed API. For production, replace CSV storage with a managed database because container filesystems may be temporary.