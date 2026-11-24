const menuButton = document.querySelector(".menu-toggle");
const nav = document.querySelector(".nav");

menuButton.addEventListener("click", () => {
  const isOpen = nav.classList.toggle("open");
  menuButton.setAttribute("aria-expanded", String(isOpen));
});

nav.querySelectorAll("a").forEach((link) => link.addEventListener("click", () => {
  nav.classList.remove("open");
  menuButton.setAttribute("aria-expanded", "false");
}));

const observer = new IntersectionObserver((entries) => {
  entries.forEach((entry) => {
    if (entry.isIntersecting) {
      entry.target.classList.add("visible");
      observer.unobserve(entry.target);
    }
  });
}, { threshold: 0.12 });

document.querySelectorAll(".reveal").forEach((element) => observer.observe(element));
document.getElementById("year").textContent = new Date().getFullYear();

const inquiryForm = document.getElementById("inquiry-form");
const statusMessage = inquiryForm.querySelector(".form-status");

inquiryForm.addEventListener("submit", async (event) => {
  event.preventDefault();
  const submitButton = inquiryForm.querySelector("button");
  const payload = Object.fromEntries(new FormData(inquiryForm));
  submitButton.disabled = true;
  statusMessage.textContent = "Sending your inquiry…";

  try {
    const apiUrl = window.JOVIA_API_URL || "";
    if (!apiUrl) {
      const subject = encodeURIComponent(`New ${payload.interest} inquiry from ${payload.name}`);
      const body = encodeURIComponent(`${payload.message}\n\nFrom: ${payload.name}\nEmail: ${payload.email}`);
      window.location.href = `mailto:joviaruth5@gmail.com?subject=${subject}&body=${body}`;
      statusMessage.textContent = "Your email app is opening—send the prepared message to finish.";
    } else {
      const response = await fetch(`${apiUrl}/api/inquiries`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(payload)
      });
      if (!response.ok) throw new Error("Request failed");
      inquiryForm.reset();
      statusMessage.textContent = "Thank you! Jovia will reply shortly.";
      statusMessage.classList.add("success");
    }
  } catch {
    statusMessage.textContent = "Something went wrong. Please email joviaruth5@gmail.com.";
  } finally {
    submitButton.disabled = false;
  }
});
