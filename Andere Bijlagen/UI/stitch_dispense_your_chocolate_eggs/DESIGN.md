```markdown
# Design System Document: The Confectioner’s Interface

## 1. Overview & Creative North Star
**Creative North Star: "The Tactile Chocolatier"**

This design system moves away from the sterile, flat aesthetic of standard kiosks to embrace a "High-End Editorial" experience tailored for a chocolate egg dispensing machine. We are not just building a menu; we are building an appetite. 

To break the "template" look on a constrained 800x480 screen, we utilize **Organic Asymmetry**. Elements are not strictly boxed; they are layered like artisanal chocolate packaging. We prioritize high-contrast readability through massive typography scales and depth through tonal shifts rather than rigid lines. The interface should feel as smooth and premium as the product it dispenses.

---

## 2. Colors & Surface Philosophy
The palette is a sophisticated journey through cocoa intensities, anchored by a vibrant, "popping" orange accent to drive conversion.

### The "No-Line" Rule
**Strict Mandate:** Prohibit 1px solid borders for sectioning. 
Structure must be defined by background shifts. For example, a flavor description sits on `surface-container-low` (#f2f1ea), resting atop a `surface` (#f8f6f0) background. This creates a soft, seamless transition that feels expensive and custom.

### Surface Hierarchy & Nesting
Treat the 800x480 canvas as a physical tray.
*   **Base Layer:** `surface` (#f8f6f0)
*   **Secondary Content Areas:** `surface-container` (#eae8e2)
*   **Elevated Interactive Cards:** `surface-container-lowest` (#ffffff)
*   **The "Glass & Gradient" Rule:** For floating "Success" or "Processing" modals, use a backdrop-blur (12px) with `surface` at 80% opacity. For primary CTAs, apply a subtle linear gradient from `primary` (#9b3f00) to `primary-container` (#ff7a2c) at a 135-degree angle to provide "soul" and a tactile, convex feel.

---

## 3. Typography: The Bold Editorial
We use **Plus Jakarta Sans** for its modern, geometric clarity and playful spirit. On a small screen, we don't scale down; we scale *up* to ensure legibility from a standing distance.

*   **Display-LG (3.5rem):** Reserved for "Choose Your Flavor" or "Enjoy!" screens. Use `-0.02em` letter spacing for a premium, tight editorial feel.
*   **Headline-MD (1.75rem):** Used for egg names (e.g., "Dark Hazelnut"). Bold weight is mandatory.
*   **Title-LG (1.375rem):** Price points and primary button labels.
*   **Body-LG (1rem):** Short ingredient descriptions. 
*   **Label-MD (0.75rem):** Nutritional micro-copy.

**Hierarchy Note:** Use `on-surface` (#2e2f2b) for headlines to ensure maximum contrast against ivory backgrounds. Use `on-surface-variant` (#5c5c57) for secondary details.

---

## 4. Elevation & Depth: Tonal Layering
Traditional shadows look muddy on small LCDs. We use **Tonal Layering** and **Ambient Light**.

*   **The Layering Principle:** To highlight a selected flavor, do not add a stroke. Instead, transition the card from `surface-container` (#eae8e2) to `surface-container-lowest` (#ffffff). The natural brightness shift provides the "lift."
*   **Ambient Shadows:** For the final "Dispense" button, use an extra-diffused shadow: `box-shadow: 0 12px 32px rgba(155, 63, 0, 0.15)`. Note the tint—the shadow uses a transparent version of the `primary` color, not black, to mimic light passing through chocolate.
*   **The "Ghost Border" Fallback:** If high-sunlight environments require more definition, use `outline-variant` (#aeada8) at **15% opacity**. It should be felt, not seen.

---

## 5. Components

### Primary Buttons (The "Dispense" Action)
*   **Shape:** `rounded-xl` (3rem) for a pill-shaped, ultra-touchable feel.
*   **Color:** Gradient from `primary` to `primary-container`.
*   **Typography:** `title-lg`, All Caps, white (`on-primary`).
*   **Interaction:** On press, the element should scale down to `0.95` to mimic physical compression.

### Flavor Selection Cards
*   **Layout:** Vertical stack. Icon/Image on top, `headline-sm` title, `body-md` description.
*   **Container:** `surface-container-low`, `rounded-lg` (2rem).
*   **Constraint:** **No dividers.** Use `spacing-6` (2rem) of vertical white space to separate the image from the text.

### Flavor Chips
*   **Usage:** For tags like "Vegan," "Nut-Free," or "Limited."
*   **Style:** `secondary-container` (#fed3c7) background with `on-secondary-container` (#65473e) text. `rounded-full`.

### The "Egg Loader" (Custom Component)
*   Instead of a generic spinner, use a pulsing circular ring in `primary-fixed-dim` (#f46800) that mimics the silhouette of an egg being prepared.

---

## 6. Do’s and Don’ts

### Do:
*   **DO** use `spacing-8` (2.75rem) between major interactive zones. On an 800px width, breathing room is your best friend.
*   **DO** use oversized iconography (48px+) for flavor profiles (cocoa beans, milk drops, nuts).
*   **DO** prioritize the `primary` orange accent only for the final "Confirm/Pay" steps to create a clear "Golden Path."

### Don’t:
*   **DON’T** use 1px dividers. If you feel the need to separate content, increase the spacing token or shift the surface color.
*   **DON’T** use pure black (#000000). It kills the warmth of the chocolate aesthetic. Use `on-background` (#2e2f2b) for "black" text.
*   **DON’T** use small touch targets. Every interactive element must be at least `spacing-12` (4rem) in height.
*   **DON’T** crowd the screen. If a flavor has too much info, use a "More Info" glassmorphic overlay instead of shrinking the font.