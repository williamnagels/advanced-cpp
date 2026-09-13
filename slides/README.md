# Building the slides

The slide decks are generated with Marp inside the project Docker image. Run
the commands below from the repository root.

## Build the image

```sh
docker build -t cpptraining -f docker/Dockerfile docker
```

If npm requires a local corporate CA, provide it as a BuildKit secret:

```sh
docker build \
  --secret id=local_ca,src=/path/to/local-ca.crt \
  -t cpptraining \
  -f docker/Dockerfile \
  docker
```

## Generate a PDF

```sh
docker run --rm \
  --user "$(id -u):$(id -g)" \
  --env HOME=/tmp \
  --env TMPDIR=/tmp \
  --mount type=bind,source="$PWD/slides",target=/slides \
  --workdir /slides \
  cpptraining \
  ./generate.sh 2_ranges.md pdf
```

The second argument is the output directory. The generated filename is based
on the Markdown filename, so this command creates `slides/pdf/2_ranges.pdf`.

## Download generated PDFs

The GitHub Actions workflow publishes these decks as the
`advanced-cpp-slides` artifact:

- `1_intro.pdf`
- `2_ranges.pdf`
- `3_coroutines.pdf`

Open the repository on GitHub, select **Actions**, open a completed
**Docker build** workflow run, and download **advanced-cpp-slides** from the
**Artifacts** section. GitHub downloads a ZIP containing the three PDFs.

Artifacts are retained for 30 days after each workflow run.